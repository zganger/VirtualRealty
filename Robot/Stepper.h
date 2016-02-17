struct Stepper
{
	int IN1;
	int IN2;
	int IN3;
	int IN4;
	int speed;
	int direction;
	int step_number;
	int last_step_time;
	int number_of_steps;
	unsigned long step_delay;
};

void stepperInit(struct Stepper, int, int, int, int, int);

void stepSpeed(struct Stepper, long);

void step(struct Stepper, int);

void stepMotor(struct Stepper, int);
