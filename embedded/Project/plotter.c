#include <math.h>
#include "motor.c"
#define PI 3.141592653589793

typedef struct
{
	u16 code; //operation code
	s16 x; //parameter 1
	s16 y; //parameter 2
}	OPER; //a plotterarm operation

/** serialized OPER
| 1  |   2   |   2   | bytes
|code|   x   |   y   | (littleEndian)
 */

typedef struct
{
	MOTOR base;
	MOTOR axis;
	MOTOR pen;
	s16 penupangle;
	u16 penupspeed;
	s16 pendownangle;
	u16 pendownspeed;
	u16 baselength;
	u16 axislength;
	float meanspeed; //the speed of the slower arm
	u16 maxspeed; //the max speed of the faster arm, or the speed when pen is up
	s16 X; //current pen position
	s16 Y;
	s16 BX[4]; //bezier points
	s16 BY[4];
	u16 num_points; //number of bezier points
	bool subroutine; //indicate whether the plotter is in a subroutine, i.e. drawing a bezier curve
	u16 register1; //registers for use in subroutine
	u16 register2;
	OPER* oper; //operation array
	s16 opercurr; //operation i
}	PLOTTER;

void PLOTTER_touchinit()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//Configure your output LED on Port F
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	//Task 2b: Configure Button pin as input PB.11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void PLOTTER_init(PLOTTER* plot)
{
	//here are all the calibrations and parameters
	(*plot) = (PLOTTER)
	{
		.base =
		{
			.timer = TIM3,
			.channel = 1,
			.max_angle =  5850,
			.min_angle = -6600,
			.max_pulse = 12500,
			.min_pulse = 5000,
			.angle = 4499,
			.to_angle = 4500,
			.angle_speed = 1,
			.angle_speed_dp1 = 0,
			.angle_speed_dp2 = 0,
			.counter = 1
		},
		.axis =
		{
			.timer = TIM3,
			.channel = 2,
			.max_angle = 14100,
			.min_angle = 0,
			.max_pulse = 13000,
			.min_pulse = 4500,
			.angle = 4499,
			.to_angle = 4500,
			.angle_speed = 1,
			.angle_speed_dp1 = 0,
			.angle_speed_dp2 = 0,
			.counter = 1
		},
		.pen =
		{
			.timer = TIM3,
			.channel = 3,
			.max_angle = 19000,
			.min_angle = -1300,
			.max_pulse = 15000,
			.min_pulse = 3000,
			.angle = 4499,
			.to_angle = 4500,
			.angle_speed = 1,
			.angle_speed_dp1 = 0,
			.angle_speed_dp2 = 0,
			.counter = 1
		},
		.penupangle = 6800,
		.penupspeed = 10,
		.pendownangle = 7800,
		.pendownspeed = 10,
		.baselength = 1400,
		.axislength = 1650,
		.meanspeed = 0.8f,
		.maxspeed = 3,
		.X = 2000,
		.Y = 0,
		.BX = {0},
		.BY = {0},
		.num_points = 0,
		.subroutine = FALSE,
		.register1 = 0,
		.register2 = 0,
		.oper = 0,
		.opercurr = -1
	};
}

void PLOTTER_testmotor(MOTOR* motor)
{
	if( motor->to_angle == motor->angle)
	{
		s16 angle = motor->to_angle;
		switch( motor->to_angle)
		{
			case 0: angle=9000; break;
			case 4500: angle=9000; break;
			case 9000: angle=0; break;
		}
		MOTOR_toangle( motor, angle, 2, 0, 0);
		LongDelay(10000000);
	}
	else
	{
		MOTOR_update( motor);
	}
}

float PLOTTER_cosine(s16 a, s16 b, s16 c)
{
	float ratio = (float)(a*a+b*b-c*c)/(2*a*b);
	//printf( "a=%d, b=%d, c=%d, ratio=%f, acos=%f\n", a,b,c,ratio,acos(ratio));
	if( ratio>1.0f || ratio<-1.0f)
	{
		printf("PLOTTER: out of range\n");
		if( ratio>0.8f)
			ratio = 1.0f;
		else if( ratio <-0.8f)
			ratio = -1.0f;
		else
			ratio = 0.0f;
	}
	return acos(ratio);
}

void PLOTTER_turn(PLOTTER* plot, s16 b_toangle, s16 a_toangle, bool fast)
{
	s16 a_div = a_toangle - plot->axis.angle;
	a_div = a_div>0 ? a_div : -a_div;
	s16 b_div = b_toangle - plot->base.angle;
	b_div = b_div>0 ? b_div : -b_div;
	if( fast)
	{
		u16 mul=1;
		if( a_div>2000 && b_div>2000)
			mul=3;
		MOTOR_toangle( &plot->axis, a_toangle, plot->maxspeed*mul, 0, 0);
		MOTOR_toangle( &plot->base, b_toangle, plot->maxspeed*mul, 0, 0);
	}
	else
	{
		float ratio;
		if( a_div < b_div)
		{
			if( a_div==0)
				a_div=1;
			ratio = (float)b_div/a_div;
		}
		else
		{
			if( b_div==0)
				b_div=1;
			ratio = (float)a_div/b_div;
		}

		u16 lspeed_dp0, lspeed_dp1, lspeed_dp2;
		u16 hspeed_dp0, hspeed_dp1, hspeed_dp2;
		{
			float lspeed = plot->meanspeed;
			float hspeed = lspeed * ratio;
			if( hspeed > plot->maxspeed)
			{
				hspeed = plot->maxspeed;
				lspeed = hspeed / ratio;
			}
			if( lspeed < 0.01f)
			  	lspeed = 0.01f;
			hspeed_dp0 =  (u16) hspeed;
			hspeed_dp1 = ((u16)(hspeed*10.0f))%10;
			hspeed_dp2 = ((u16)(hspeed*100.0f))%10;
			lspeed_dp0 =  (u16) lspeed;
			lspeed_dp1 = ((u16)(lspeed*10.0f))%10;
			lspeed_dp2 = ((u16)(lspeed*100.0f))%10;
		}

		if( a_div < b_div)
		{
			MOTOR_toangle( &plot->axis, a_toangle, lspeed_dp0, lspeed_dp1, lspeed_dp2);
			MOTOR_toangle( &plot->base, b_toangle, hspeed_dp0, hspeed_dp1, hspeed_dp2);
			//printf( "a_div=%d, b_div=%d, speed axis=%d, speed base=%d.%d%d\n", a_div, b_div, lowspeed, speed, speed_dp1, speed_dp2);
		}
		else
		{
			MOTOR_toangle( &plot->axis, a_toangle, hspeed_dp0, hspeed_dp1, hspeed_dp2);
			MOTOR_toangle( &plot->base, b_toangle, lspeed_dp0, lspeed_dp1, lspeed_dp2);
			//printf( "a_div=%d, b_div=%d, speed axis=%d.%d%d, speed base=%d\n", a_div, b_div, speed, speed_dp1, speed_dp2, lowspeed);
		}
	}
}

void PLOTTER_move(PLOTTER* plot, s16 x, s16 y, bool fast) //move to an absolute coordinate
{
	plot->X = x;
	plot->Y = y;
	u16 length = (u16)sqrt(x*x + y*y);
	float the1 = PLOTTER_cosine( length, plot->baselength, plot->axislength);
	float the2 = PLOTTER_cosine( plot->axislength, plot->baselength, length);
	float the3 = atan( (float)y/x );
	//printf( "y=%d, x=%d, ratio=%f, atan=%f\n", y,x,(float)y/x, the3);
	float beta;
	if( x>=0)
		beta = PI/2 - the3 - the1;
	else
		beta = -the3 - PI/2 - the1;
	float alpha = PI - the2;
	//printf( "the1=%f, the2=%f, the3=%f, beta=%f, alpha=%f\n",the1,the2,the3,beta,alpha);

	s16 a_toangle = (s16)(alpha/PI*18000);
	s16 b_toangle = (s16)(beta/PI*18000);
	//printf( "alpha=%f, a=%d, beta=%f, b=%d\n",alpha,a_toangle,beta,b_toangle);
	if( a_toangle > plot->axis.max_angle ||
		b_toangle > plot->base.max_angle ||
		a_toangle < plot->axis.min_angle ||
		b_toangle < plot->base.min_angle )
		x = y;

	PLOTTER_turn(plot, b_toangle, a_toangle, fast);
}

void PLOTTER_penupdown(PLOTTER* plot, u8 down)
{
	//choose the faster motor
	MOTOR* mo;
	if( plot->base.angle_speed > plot->axis.angle_speed)
		mo=&plot->base;
	else if( plot->base.angle_speed_dp1 > plot->axis.angle_speed_dp1)
		mo=&plot->base;
	else if( plot->base.angle_speed_dp2 > plot->axis.angle_speed_dp2)
		mo=&plot->base;
	else
		mo=&plot->base;

	u16 tdiff;
	{
		float mspeed = mo->angle_speed + (mo->angle_speed_dp1)*0.1f + (mo->angle_speed_dp2)*0.01f;
		s16 m_div = mo->angle - mo->to_angle;
		m_div = m_div>0 ? m_div : -m_div;
		tdiff = (u16) (m_div/mspeed+0.5f);
	}
	u16 pspeed_dp0, pspeed_dp1, pspeed_dp2;
	{
		float pspeed = (float)(plot->pendownangle-plot->penupangle)/tdiff;
		pspeed_dp0 =  (u16) pspeed;
		pspeed_dp1 = ((u16)(pspeed*10.0f))%10;
		pspeed_dp2 = ((u16)(pspeed*100.0f))%10;
	}
	if( down)
		MOTOR_toangle( &plot->pen, plot->pendownangle, pspeed_dp0, pspeed_dp1, pspeed_dp2);
	else
		MOTOR_toangle( &plot->pen, plot->penupangle, pspeed_dp0, pspeed_dp1, pspeed_dp2);
}

void PLOTTER_pendown(PLOTTER* plot)
{
	MOTOR_toangle( &plot->pen, plot->pendownangle, plot->pendownspeed, 0, 0);
}

void PLOTTER_penstop(PLOTTER* plot)
{
	plot->pen.to_angle = plot->pen.angle;
}

void PLOTTER_penup(PLOTTER* plot)
{
	MOTOR_toangle( &plot->pen, plot->penupangle, plot->penupspeed, 0, 0);
}

s16 PLOTTER_getstep(s16 x1, s16 x2, u16 stepcount, u16 numofsteps)
{
	return ((numofsteps - stepcount) * x1 + stepcount * x2) / numofsteps;
}

void PLOTTER_operation(PLOTTER* plot, OPER* oper); //declare

void PLOTTER_subroutine_bezier(PLOTTER* plot)
{
	if( plot->register1 < plot->register2)
	{
		#define STEP(X1,X2) PLOTTER_getstep(X1, X2, plot->register1, plot->register2)
		#define STEPBX(i,j) PLOTTER_getstep(plot->BX[i], plot->BX[j], plot->register1, plot->register2)
		#define STEPBY(i,j) PLOTTER_getstep(plot->BY[i], plot->BY[j], plot->register1, plot->register2)
		s16 x,y;
		if( plot->num_points == 3)
		{	//quadratic
			x = STEP(STEPBX(0,1),STEPBX(1,2));
			y = STEP(STEPBY(0,1),STEPBY(1,2));
		}
		else if( plot->num_points == 4)
		{	//cubic
			x = STEP(STEP(STEPBX(0,1),STEPBX(1,2)),STEP(STEPBX(1,2),STEPBX(2,3)));
			y = STEP(STEP(STEPBY(0,1),STEPBY(1,2)),STEP(STEPBY(1,2),STEPBY(2,3)));
		}
		OPER oper={'L',x,y};
		printf("(%d,%d) ",x,y);
		PLOTTER_operation(plot, &oper);
		plot->register1++;
		#undef STEP
		#undef STEPBX
		#undef STEPBY
	}
	else
	{
		OPER oper={'L',plot->BX[plot->num_points-1],plot->BY[plot->num_points-1]};
		PLOTTER_operation(plot, &oper);
		plot->subroutine = FALSE;
		plot->num_points = 0;
	}
}

void PLOTTER_calibrate(PLOTTER* plot, OPER* oper)
{
	u8 hbyte = (u8)(oper->x>>8);
	u8 lbyte = (u8)(oper->x & 0xFF);
	MOTOR* mo=0;
	switch (lbyte)
	{
		case 'b': mo = &(plot->base); break;
		case 'a': mo = &(plot->axis); break;
		case 'p': mo = &(plot->pen); break;
	}
	if( mo)
	switch (hbyte)
	{
		case 'P': mo->max_pulse = oper->y; break;
		case 'p': mo->min_pulse = oper->y; break;
		case 'A': mo->max_angle = oper->y; break;
		case 'a': mo->min_angle = oper->y; break;
		case 'W': MOTOR_update_pulsewidth(mo, (u16)oper->y); break;
		case 'R': MOTOR_toangle( mo, oper->y, 20000, 0, 0); MOTOR_update(mo); break;
		case 'L':
			if(lbyte=='b') plot->baselength = oper->y;
			if(lbyte=='a') plot->axislength = oper->y;
		break;
		case 'D': if(lbyte=='p') plot->pendownangle = oper->y; break;
		case 'd': if(lbyte=='p') plot->penupangle = oper->y; break;
		case 'S': if(lbyte=='p') plot->pendownspeed = oper->y; break;
		case 's': if(lbyte=='p') plot->penupspeed = oper->y; break;
	}
	//trigger PLOTTER_signal on next update
	plot->base.angle=plot->base.to_angle;
	plot->axis.angle=plot->axis.to_angle;
	plot->pen.angle=plot->pen.to_angle;
}

void PLOTTER_operation(PLOTTER* plot, OPER* oper)
{
	switch (oper->code)
	{
		//move absolutely
		case 'M':
		PLOTTER_move(plot, oper->x, oper->y, TRUE);
		break;

		case 'L':
		{
			OPER dump={'l', oper->x-plot->X, oper->y-plot->Y}; //compute as relative move
			PLOTTER_operation(plot, &dump);
			return;
		}

		//move relatively
		case 'm':
		PLOTTER_move(plot, plot->X+oper->x, plot->Y+oper->y, TRUE);
		break;

		case 'l':
		{
			s16 ext;
			if( !plot->subroutine)
				ext=20; //extends over where it is supposed to end
			else
				ext=10;
			s16 holdX = plot->X+oper->x;
			s16 holdY = plot->Y+oper->y;
			s16 len = (s16)sqrt(oper->x*oper->x + oper->y*oper->y);
			PLOTTER_move(plot, plot->X+oper->x+oper->x*ext/len, plot->Y+oper->y+oper->y*ext/len, FALSE);
			plot->X = holdX;
			plot->Y = holdY;
		}
		break;

		//turn to angle
		case 'A':
		PLOTTER_turn(plot, oper->x, oper->y, FALSE);
		break;

		case 'a':
		PLOTTER_turn(plot, plot->base.angle+oper->x, plot->axis.angle+oper->y, FALSE);
		break;

		//the control points of bezier
		case 'B':
		if( plot->num_points == 0)
		{
			plot->BX[0] = plot->X;
			plot->BY[0] = plot->Y;
			plot->BX[1] = oper->x;
			plot->BY[1] = oper->y;
			plot->num_points=2;
		}
		else if( plot->num_points == 2)
		{
			plot->BX[2] = oper->x;
			plot->BY[2] = oper->y;
			plot->num_points=3;
		}
		break;
		case 'b':
		if( plot->num_points == 0)
		{
			plot->BX[0] = plot->X;
			plot->BY[0] = plot->Y;
			plot->BX[1] = plot->X + oper->x;
			plot->BY[1] = plot->Y + oper->y;
			plot->num_points=2;
		}
		else if( plot->num_points == 2)
		{
			plot->BX[2] = plot->X + oper->x;
			plot->BY[2] = plot->Y + oper->y;
			plot->num_points=3;
		}
		break;

		//draw a bezier
		case 'C': case 'c':
		{
			#define DISTANCE(i,j) ((u16)sqrt((plot->BX[i]-plot->BX[j])*(plot->BX[i]-plot->BX[j])+(plot->BY[i]-plot->BY[j])*(plot->BY[i]-plot->BY[j])))
			const u16 minlength = 50;
			u16 dist;

			if( oper->code=='C')
			{
				plot->BX[plot->num_points] = oper->x;
				plot->BY[plot->num_points] = oper->y;
			}
			else
			{
				plot->BX[plot->num_points] = plot->X + oper->x;
				plot->BY[plot->num_points] = plot->Y + oper->y;
			}
			if( plot->num_points == 2) //quadratic
				dist = DISTANCE(0,1)+DISTANCE(1,2);
			else if( plot->num_points == 3) //cubic
				dist = (DISTANCE(0,1)+DISTANCE(2,3)+DISTANCE(0,2)+DISTANCE(1,3))/2;

			plot->register1 = 0;
			plot->register2 = dist / minlength;
			plot->subroutine = TRUE;
			plot->num_points++;
			PLOTTER_subroutine_bezier(plot);
			printf("B0(%d,%d), B1(%d,%d), B2(%d,%d), B3(%d,%d), num_points=%d, seg=%d\n", plot->BX[0],plot->BY[0],plot->BX[1],plot->BY[1],plot->BX[2],plot->BY[2],plot->BX[3],plot->BY[3],plot->num_points,plot->register2);
			#undef DISTANCE
		}
		break;
	}

	switch (oper->code)
	{
		case 'M': case 'm':
			PLOTTER_penup(plot);
		break;

		case 'L': case 'l': case 'C': case 'c':
			PLOTTER_pendown(plot);
		break;
	}
}

u8 PLOTTER_signal(PLOTTER* plot) //signal that one operation has been finished
{
	OPER* oper=&plot->oper[plot->opercurr];
	if( !plot->subroutine)
	{
		plot->opercurr++;
		oper=&plot->oper[plot->opercurr];
		switch( oper->code)
		{
			case '~': //calibration
				PLOTTER_calibrate(plot, oper);
			break;

			case '^': //control
				switch((u8)oper->x)
				{
					case 'P': //end of page, wait for next page
					return 'P';
					case 'Q': //end of file
					MOTOR_toangle(&plot->pen, plot->penupangle, 20000, 0, 0);
					MOTOR_update(&plot->pen);
					return 'Q';
				}
			break;

			default:
				PLOTTER_operation(plot, oper); //perform operation
		}
	}
	else
	{	//a sub step of subroutine has been finished
		switch( oper->code)
		{
			case 'C': case 'c':
				PLOTTER_subroutine_bezier(plot); //next step of subroutine
			break;
		}
	}
	return oper->code;
}

u8 PLOTTER_update(PLOTTER* plot)
{
	if( (plot->base.angle==plot->base.to_angle) &&
		(plot->axis.angle==plot->axis.to_angle) &&
		(plot->pen.angle==plot->pen.to_angle) )
	{	//all three motors reached destination angle
		//if( !plot->subroutine)
			//LongDelay(5000);
		return PLOTTER_signal(plot);
	}

	if( plot->pen.angle==plot->pen.to_angle)
	{
		MOTOR_update(&plot->pen);
		MOTOR_update(&plot->base);
		MOTOR_update(&plot->axis);
	}
	else
		MOTOR_update(&plot->pen);

	/* OPER* oper = &plot->oper[plot->opercurr];
	if( oper->code=='L' || oper->code=='l')
	{
		if( plot->penisdown)
			PLOTTER_penstop(plot);
	}*/

	if( !plot->subroutine)
		LongDelay(5000);
	else
		LongDelay(5000);
	return 0;
}
