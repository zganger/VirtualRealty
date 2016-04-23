
from time import sleep
import RPi.GPIO as GPIO

# GPIO pins of sensors
GPIO.setmode(GPIO.BCM)
GPIO_in = 16
GPIO_out = 20
GPIO.setup(GPIO_in,GPIO.IN)
GPIO.setup(GPIO_out, GPIO.OUT)

delay=.2;

while True:
	# Read sensor input and print some diagnostics
	reading = GPIO.input(GPIO_in)
	if reading == 1:
		GPIO.output(GPIO_out,True)
	elif reading == 0:
		GPIO.output(GPIO_out,False)
	sleep(delay)
