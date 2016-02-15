struct Stepper
{
	int IN1;
	int IN2;
	int IN3;
	int IN4;
	int speed;
	int direction=0;
	int step_number=0;
	int last_step_time=0;
	int number_of_steps;
	unsigned long step_delay;
};

void stepperInit(struct Stepper, int, int, int, int, int);

void stepSpeed(struct Stepper, long);

void step(struct Stepper, int);

void stepMotor(int);