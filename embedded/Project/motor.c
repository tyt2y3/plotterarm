#ifndef MOTOR_C
#define MOTOR_C

/** a class to control multiple servo motors concurrently
	- each MOTOR outputs a PWM signal to a GPIO port
	- calibration of operational range
	- control a servo motor to rotate to an absolute angle with precise speed up to 2 decimal places
	- do integer computation only
 */

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "pwm.c"

typedef u8 (*MOTOR_CALLBACK) (u8 flag, s16 value);

typedef struct
{
	TIM_TypeDef* timer;	//only use TIM3
	u8 channel;			//can be 1,2,3 or 4
	s32 max_angle;		//
	s32 min_angle;		//
	u16 max_pulse;		//the max pulse width to attain max angle
	u16 min_pulse;		//the min pulse width to attain min angle
	s16 angle;			//current angle
	s16 to_angle;		//target angle
	u16 angle_speed;	//turning speed of the motor, integer
	u16 angle_speed_dp1;//speed at 1 decimal place, will be added to displacement once every 10 update
	u16 angle_speed_dp2;//speed at 2 decimal place, will be added to displacement once every 100 update
	u16 counter;		//iteration counter
}	MOTOR;

void MOTOR_toangle(MOTOR* M, s16 to_angle, u16 speed, u16 speed_dp1, u16 speed_dp2)
{
	if( to_angle < M->min_angle)
	{
		M->to_angle = M->min_angle;
		printf("MOTOR: angle too small\n");
	}
	else if( to_angle > M->max_angle)
	{
		M->to_angle = M->max_angle;
		printf("MOTOR: angle too large\n");
	}
	else
		M->to_angle = to_angle;

	M->angle_speed = speed;
	M->angle_speed_dp1 = speed_dp1;
	M->angle_speed_dp2 = speed_dp2;
	M->counter=1;
}

void MOTOR_update_pulsewidth(MOTOR* M, u16 CCR)
{
	switch( M->channel)
	{
		case 1: M->timer->CCR1 = CCR; break;
		case 2: M->timer->CCR2 = CCR; break;
		case 3: M->timer->CCR3 = CCR; break;
		case 4: M->timer->CCR4 = CCR; break;
	}
}

/** must be called manually at a fixed interval
 */
void MOTOR_update(MOTOR* M)
{
	s16 div  = M->to_angle - M->angle;	//difference
	s16 diva = div > 0 ? div : -div;	//absolute

	if( div != 0)
	{
		if( diva < M->angle_speed ||
			(M->counter%10==0 && diva < M->angle_speed_dp1) ||
			(M->counter%100==0 && diva < M->angle_speed_dp2))
			M->angle = M->to_angle;
		else
		{
			/* //slow down to prevent overshoot
			const s16 thres = 100;
			if( (M->angle < M->to_angle && M->angle > M->to_angle-thres) ||
				(M->angle > M->to_angle && M->angle < M->to_angle+thres) )
				if( M->angle_speed > 1)
					M->angle_speed--; */
			
			s16 offset = M->angle_speed;
			if( M->counter%10==0)
				offset += M->angle_speed_dp1;
			if( M->counter%100==0)
				offset += M->angle_speed_dp2;
			
			M->angle += div>0? offset:-offset;
		}
		u16 width = M->max_pulse - ((M->max_angle - M->angle) * (M->max_pulse - M->min_pulse) / (M->max_angle - M->min_angle));
		MOTOR_update_pulsewidth(M, width);
		
		M->counter++;
		if( M->counter==101)
			M->counter=1;
	}
	else
	{
		//motor reached target angle
	}
}

#endif
