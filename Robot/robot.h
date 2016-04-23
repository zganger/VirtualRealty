#include "Stepper.h"

int init();
int initSensors();
int initBluetooth();
int initVariables();

string getBluetooth();
string decodeBluetooth(string);

void startMotors(struct Stepper);
int checkDestination();
char pollLineFollowing();
int LineDetected();
int objectDetected();

int checkCorner();
void turnCorner();

void stopMotors(struct Stepper);

void sendBluetooth(string);
