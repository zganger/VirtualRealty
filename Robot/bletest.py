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

print("Beginning...")
ble.begin()
print("Resetting...")
ble.factoryReset()
print("Connecting...")
while(not ble.isConnected()):
	#print("here")
	print("Failed to Connect")
	time.sleep(.5)

print("Connected!")
while(True):
	ble.println("AT+BLEUARTRX")
	rx=ble.readLine()
	#print(rx)
	if(rx=="OK"):
		continue
	if(rx!=None):
		print(rx)
	ble.waitForOK()
	#time.sleep(1)
