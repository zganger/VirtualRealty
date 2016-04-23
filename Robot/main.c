#include "Robot.h"
#include <string.h>
void main()
{
	int counter,speed;
	string recvCMD,decodedCMD,sentCMD;
	char direction;
	if(init())
	{
		counter=0;
		recvCMD=getBluetooth();
		decodedCMD=decodeBluetooth(rawBluetooth);
		while(checkDestination(decodeBlue,counter))
		{
			if(!objectDetected())
			{
				startMotors(direction,speed); //where to get speed?
				pollLineFollowing(); //possible issues /**HERE**/
				if(LineDetected())
				{
					counter++;
				}
				if(checkCorner())
				{
					turnCorner();
				}					/**TO HERE**/
			}
			else
			{
				stopMotors();
			}
		}
		stopMotors();
		sendBluetooth(sentCMD);
	
	}

}