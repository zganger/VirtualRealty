
bool init();
bool initSensors();
bool initBluetooth();
bool initVariables();

string getBluetooth();
string decodeBluetooth(string);

void startMotors(char,int);
bool checkDestination();
char pollLineFollowing();
bool LineDetected();
bool objectDetected();

bool checkCorner();
void turnCorner();

void stopMotors();

void sendBluetooth(string);
