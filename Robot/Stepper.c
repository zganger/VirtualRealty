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
	/*** TO DO
	set pin as output
	***/
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
		/*
		s1.IN1=HIGH;
		s1.IN2=LOW;
		s1.IN3=HIGH;
		s1.IN4=LOW;		
		*/
		break;
		case 1: //0110
		/*
		s1.IN1=LOW;
		s1.IN2=HIGH;
		s1.IN3=HIGH;
		s1.IN4=LOW;		
		*/
		break;
		case 2: //0101
		/*
		s1.IN1=LOW;
		s1.IN2=HIGH;
		s1.IN3=LOW;
		s1.IN4=HIGH;		
		*/
		break;
		case 3: //1001
		/*
		s1.IN1=HIGH;
		s1.IN2=LOW;
		s1.IN3=LOW;
		s1.IN4=HIGH;		
		*/
		break;
	}
}
