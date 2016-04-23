import time
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
trig = 12
echo = 25

GPIO.setup(trig, GPIO.OUT)
GPIO.setup(echo, GPIO.IN)

trig_delay=.00001
read_delay = .5

GPIO.output(trig, False)
time.sleep(2)
print("starting measurements")

try:
	while True:
		GPIO.output(trig, True)
		time.sleep(trig_delay)
		GPIO.output(trig, False)

		while GPIO.input(echo) == 0:
			read_start = time.time()
		while GPIO.input(echo) == 1:
			read_end = time.time()

		read_duration = read_end - read_start
		print("read duration time in seconds: ",read_duration)
		distance = read_duration*17150
		print("distance in cm: ",distance)
		time.sleep(read_delay)
finally:
	print("in final signal handler")
	GPIO.cleanup()
	exit(0)
