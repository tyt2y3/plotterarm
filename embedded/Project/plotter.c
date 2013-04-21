#include <math.h>
#include "motor.c"
#define PI 3.141592653589793

typedef struct
{
	u8 code; //operation code
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
	u16 meanspeed; //the speed of the slower arm
	u16 maxspeed; //the max speed of the faster arm, or the speed when pen is up
	s16 X; //current pen position
	s16 Y;
	s16 AX; //base point of bezier2
	s16 AY;
	s16 BX; //control point of bezier2
	s16 BY;
	s16 CX; //third point of bezier2
	s16 CY;
	bool subroutine; //indicate whether the plotter is in a subroutine, i.e. drawing a bezier curve
	u16 register1; //registers for use in subroutine
	u16 register2; //
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
			.max_angle =  13800,
			.min_angle = -10600,
			.max_pulse = 8000,
			.min_pulse = 1,
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
			.max_angle = 15360,
			.min_angle = -10040,
			.max_pulse = 8000,
			.min_pulse = 1,
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
			.max_angle = 9550,
			.min_angle = -15100,
			.max_pulse = 8000,
			.min_pulse = 1,
			.angle = -2999,
			.to_angle = -3000,
			.angle_speed = 1,
			.angle_speed_dp1 = 0,
			.angle_speed_dp2 = 0,
			.counter = 1
		},
		.penupangle = -1400,
		.penupspeed = 500,
		.pendownangle = -300,
		.pendownspeed = 25,
		.baselength = 1400,
		.axislength = 1590,
		.meanspeed = 1,
		.maxspeed = 4,
		.X = 2000,
		.Y = 0,
		.AX = 0,
		.AY = 0,
		.BX = 0,
		.BY = 0,
		.CX = 0,
		.CY = 0,
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
	float ratio = (float)(a*a+b*b-c*c)/(float)(2*a*b);
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
	if( fast)
	{
		MOTOR_toangle( &plot->axis, a_toangle, plot->maxspeed, 0, 0);
		MOTOR_toangle( &plot->base, b_toangle, plot->maxspeed, 0, 0);
	}
	else
	{
		s16 a_div = a_toangle - plot->axis.angle;
		a_div = a_div>0 ? a_div : -a_div;
		s16 b_div = b_toangle - plot->base.angle;
		b_div = b_div>0 ? b_div : -b_div;

		u16 speed, speed_dp1, speed_dp2;
		u16 lowspeed = plot->meanspeed;
		float ratio;

		if( a_div < b_div)
		{
			if( a_div==0)
				a_div=1;
			ratio = (float)b_div/(float)a_div;
		}
		else
		{
			if( b_div==0)
				b_div=1;
			ratio = (float)a_div/(float)b_div;
		}

		float fspeed = (float)lowspeed * ratio;
		speed = (u16)fspeed;
		speed_dp1 = ((u16)(fspeed*10.0f))%10;
		speed_dp2 = ((u16)(fspeed*100.0f))%10;
		if( speed > plot->maxspeed)
			speed = plot->maxspeed;

		if( a_div < b_div)
		{
			MOTOR_toangle( &plot->axis, a_toangle, lowspeed, 0, 0);
			MOTOR_toangle( &plot->base, b_toangle, speed, speed_dp1, speed_dp2);
			//printf( "a_div=%d, b_div=%d, speed axis=%d, speed base=%d.%d%d\n", a_div, b_div, lowspeed, speed, speed_dp1, speed_dp2);
		}
		else
		{
			MOTOR_toangle( &plot->axis, a_toangle, speed, speed_dp1, speed_dp2);
			MOTOR_toangle( &plot->base, b_toangle, lowspeed, 0, 0);
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
	float the3 = atan( (float)y/(float)x );
	//printf( "y=%d, x=%d, ratio=%f, atan=%f\n", y,x,(float)y/(float)x, the3);
	float beta = PI/2 - the3 - the1;
	float alpha = PI - the2;
	//printf( "the1=%f, the2=%f, the3=%f, beta=%f, alpha=%f\n",the1,the2,the3,beta,alpha);

	s16 a_toangle = (s16)(alpha/PI*18000);
	s16 b_toangle = (s16)(beta/PI*18000);
	//printf( "alpha=%f, a=%d, beta=%f, b=%d\n",alpha,a_toangle,beta,b_toangle);

	PLOTTER_turn(plot, b_toangle, a_toangle, fast);
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

void PLOTTER_subroutine_bezier2(PLOTTER* plot)
{
	if( plot->register1 < plot->register2)
	{
		s16 x = PLOTTER_getstep(PLOTTER_getstep(plot->AX, plot->BX, plot->register1, plot->register2), PLOTTER_getstep(plot->BX, plot->CX, plot->register1, plot->register2), plot->register1, plot->register2);
		s16 y = PLOTTER_getstep(PLOTTER_getstep(plot->AY, plot->BY, plot->register1, plot->register2), PLOTTER_getstep(plot->BY, plot->CY, plot->register1, plot->register2), plot->register1, plot->register2);
		OPER oper={'L',x,y};
		PLOTTER_operation(plot, &oper);
		plot->register1++;
	}
	else
	{
		OPER oper={'L',plot->CX,plot->CY};
		PLOTTER_operation(plot, &oper);
		plot->subroutine = FALSE;
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
			s16 ext; //bad hack, extends over where it is supposed to end
			if( !plot->subroutine)
				ext = 30;
			else
				ext = 20;

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

		//the second point of bezier
		case 'B':
		plot->AX = plot->X;
		plot->AY = plot->Y;
		plot->BX = oper->x;
		plot->BY = oper->y;
		break;
		case 'b':
		plot->AX = plot->X;
		plot->AY = plot->Y;
		plot->BX = plot->X + oper->x;
		plot->BY = plot->Y + oper->y;
		break;

		//draw a bezier curve!
		case 'C': case 'c':
		if( oper->code=='C')
		{
			plot->CX = oper->x;
			plot->CY = oper->y;
		}
		else
		{
			plot->CX = plot->X + oper->x;
			plot->CY = plot->Y + oper->y;
		}
		{
			const u16 minlength = 50;
			u16 dist = (u16)sqrt((plot->X-plot->CX)*(plot->X-plot->CX) + (plot->Y-plot->CY)*(plot->Y-plot->CY)) +
						(u16)sqrt((plot->BX-plot->CX)*(plot->BX-plot->CX) + (plot->BY-plot->CY)*(plot->BY-plot->CY));
			u16 steps = dist / minlength;
			//printf("steps=%d\n", steps);
			plot->register1 = 0;
			plot->register2 = steps;
			plot->subroutine = TRUE;
			PLOTTER_subroutine_bezier2(plot);
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
				if( (u8)oper->x=='Q')
				{
					//plot->opercurr = -1; //repeat all the operations again
					return 'Q';
				}
			break;

			default:
				PLOTTER_operation(plot, oper); //perform operation
		}
	}
	else
	{	//an operational step of a subroutine has been finished
		switch( oper->code)
		{
			case 'C': case 'c':
				PLOTTER_subroutine_bezier2(plot); //next step of subroutine
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
		if( !plot->subroutine)
			LongDelay(5000);
		return PLOTTER_signal(plot);
	}

	if( plot->pen.angle==plot->pen.to_angle)
	{
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
