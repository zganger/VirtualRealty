#include "Robot.h"
#include "Stepper.h"

int init()
{
	int sensorFailCount=0;
	int BTFailCount=0;
	while(!initSensors() && sensorFailCount<5)
	{
		sensorFailCount++;
	}
	while(!initBluetooth() && BTFailCount<5)
	{
		BTFailCount++;
	}
	initVariables();
	return 0;
}

void stopMotors(struct Stepper s1)
{
	stepSpeed(s1,0); //speed set to zero
	step(s1,0); //sets to take set to 0
}
	