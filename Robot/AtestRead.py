import time
import RPi.GPIO as GPIO

# GPIO pins of sensors
GPIO.setmode(GPIO.BCM)
rightSensor = 16
middleSensor = 20
leftSensor = 21
rightLED = 13
middleLED = 19
leftLED = 26
GPIO.setup(rightSensor, GPIO.IN)
GPIO.setup(middleSensor, GPIO.IN)
GPIO.setup(leftSensor, GPIO.IN)
#GPIO.setup(rightLED, GPIO.OUT)
#GPIO.setup(middleLED, GPIO.OUT)
#GPIO.setup(leftLED, GPIO.OUT)

delay=1;

while True:
	# Read sensor input and print some diagnostics
	right = GPIO.input(rightSensor)
	#middle = GPIO.input(middleSensor)
	#left = GPIO.input(leftSensor)
	print(right)	
	time.sleep(delay)
