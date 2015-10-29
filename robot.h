
void initSensors();
void initBluetooth();
void initVariables();

string getBluetooth();
string decodeBluetooth(string);

void startMotors(int);
bool checkDestination();
int pollLineFollowing();
bool LineDetected();

bool objectDetected();

void stopMotors();

void sendBluetooth(string);
