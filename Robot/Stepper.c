#include "Stepper.h"
#include <stdlib.h>
struct Stepper;

void stepperInit(struct Stepper s1, int num_steps, int P1, int P2, int P3,int P4)
{
	s1.IN1=P1;
	s1.IN2=P2;
	s1.IN3=P3;
	s1.IN4=P4;
	s1.direction=0;
	s1.step_number=0;
	s1.last_step_time=0;


}


void stepSpeed(struct Stepper s1, long setSpeed)
{
	
	s1.step_delay= 60L * 1000L * 1000L / s1.number_of_steps / setSpeed;
	
}

void step(struct Stepper s1, int steps_to_move)
{
	
	int steps_left = abs(steps_to_move);
	
	//sets direction
	if(steps_to_move > 0)
		s1.direction = 1;
	if(steps_to_move < 0)
		s1.direction = 0;
	
	while(steps_left > 0)
	{
		//unsigned long now = micros() //**** get micros with kinetis *****
		unsigned long now;
		if(now - s1.last_step_time >= s1.step_delay)
		{
			s1.last_step_time = now;
			if (s1.direction == 1)
			{
				s1.step_number++;
				if(s1.step_number == s1.number_of_steps)
				{
					s1.step_number = 0;
				}
			}
			else
			{
				if(s1.step_number == 0)
				{
					s1.step_number = s1.number_of_steps;
				}
				s1.step_number--;
			}
			steps_left--;
			stepMotor(s1,s1.step_number%4);
		}
	}
}

void stepMotor(struct Stepper s1, int thisStep)
{
	switch(thisStep)
	{
		case 0: //1010
		IN1_SetVal();
		IN2_ClrVal();
		IN3_SetVal();
		IN4_ClrVal();

		break;
		case 1: //0110
		IN1_ClrVal();
		IN2_SetVal();
		IN3_SetVal();
		IN4_ClrVal();

		break;
		case 2: //0101

		IN1_ClrVal();
		IN2_SetVal();
		IN3_ClrVal();
		IN4_SetVal();

		break;
		case 3: //1001

		IN1_SetVal();
		IN2_ClrVal();
		IN3_ClrVal();
		IN4_SetVal();

		break;
	}
}
