import BLE as ble
import signal
import RPi.GPIO as GPIO
import time
def sigterm_handler(signal,frame):
	ble.end()
	GPIO.cleanup()
	print("cleaning up gpio from signal handler")
	exit(0)
signal.signal(signal.SIGINT, sigterm_handler)
signal.signal(signal.SIGTERM, sigterm_handler)

def decodeBluetooth(n):
        return(int(n[0]),int(n[1:3]),int(n[3:]))

print("Beginning...")
ble.begin()
print("Resetting...")
ble.factoryReset()
print("Waiting for Input...")
"""while(not ble.isConnected()): #ble.isConnected()):
	
        #print("here")
	print("Not Connected")
	time.sleep(1)
	ble.connect=ble.isConnected()
	if(ble.connect):
		break
"""
#print("Connected!")
while(True):
	ble.println("AT+BLEUARTRX")
	rx=ble.readLine()
	#print(rx)
	#print(ble.rxText)
	if(rx=="OK\r\n"):
		continue
	if(ble.rxText!=""):
		print(ble.rxText)
		ble.rx=""
		#ble.disconnect()
	#time.sleep(1)
		break
	ble.waitForOK()

print("Done")

#pID,moves,corner1=decodeBluetooth(ble.rxText)
