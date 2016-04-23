import time
import RPi.GPIO as GPIO
from sys import exit
import signal

# GPIO pins of sensors
GPIO.setmode(GPIO.BCM)
rightSensor = 16
middleSensor = 20
leftSensor = 21
#rightLED = 13
#middleLED = 19
#leftLED = 26
GPIO.setup(rightSensor, GPIO.IN)
GPIO.setup(middleSensor, GPIO.IN)
GPIO.setup(leftSensor, GPIO.IN)
#GPIO.setup(rightLED, GPIO.OUT)
#GPIO.setup(middleLED, GPIO.OUT)
#GPIO.setup(leftLED, GPIO.OUT)

L_start = 0
L_end = 0
M_start = 0
M_end = 0
R_start = 0
R_end = 0

L=1
M=1
R=1

delay=1

def sigterm_handler(signal,frame):
	GPIO.cleanup()
	print("cleaning up gpio from signal handler")
	exit(0)
signal.signal(signal.SIGINT, sigterm_handler)
signal.signal(signal.SIGTERM, sigterm_handler)
while True:
	# Read sensor input and print some diagnostics
	if (L ==1):
		#read left sensor
		GPIO.setup(leftSensor, GPIO.OUT)
		GPIO.output(leftSensor, True)
		L_start = time.time()
		GPIO.setup(leftSensor, GPIO.IN)
		while GPIO.input(leftSensor) == 1:
			L_end = time.time()
	if (M==1):
		#read middle sensor
		GPIO.setup(middleSensor, GPIO.OUT)
		GPIO.output(middleSensor, True)
		M_start = time.time()
		GPIO.setup(middleSensor, GPIO.IN)
		while GPIO.input(middleSensor) == 1:
			M_end = time.time()
	if (R ==1):
		#read right sensor
		GPIO.setup(rightSensor, GPIO.OUT)
		GPIO.output(rightSensor, True)
		R_start = time.time()
		GPIO.setup(rightSensor, GPIO.IN)
		while GPIO.input(rightSensor) == 1:
			R_end = time.time()
	L_pulse = (L_end-L_start)*1000
	print("Left sensor pulse: ", L_pulse)
	M_pulse = (M_end-M_start)*1000
	print("middle sensor pulse: ", M_pulse)
	R_pulse = (R_end-R_start)*1000
	print("right sensor pulse: ", R_pulse)
	time.sleep(delay)
