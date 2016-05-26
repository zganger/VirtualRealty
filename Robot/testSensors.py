import time
import RPi.GPIO as GPIO
import signal

GPIO.setmode(GPIO.BCM)

#setup GPIOs for ultrasonic sensor
trig = 12
echo = 25
GPIO.setup(trig, GPIO.OUT)
GPIO.setup(echo, GPIO.IN)

#setup GPIO for line following IR sensors
right_ir =21
middle_ir =20
left_ir =16
GPIO.setup(right_ir, GPIO.IN)
GPIO.setup(middle_ir, GPIO.IN)
GPIO.setup(left_ir, GPIO.IN)

#setup GPIO for space counting IR sensors
side1_ir =26
side2_ir =19
side3_ir =13
GPIO.setup(side1_ir, GPIO.IN)
GPIO.setup(side2_ir, GPIO.IN)
GPIO.setup(side2_ir, GPIO.IN)

# LED indicator GPIOs
rightLED = 18
leftLED = 23
moveLED = 24
GPIO.setup(rightLED, GPIO.OUT)
GPIO.setup(leftLED, GPIO.OUT)
GPIO.setup(moveLED, GPIO.OUT)

#motor controller for left wheel GPIO setup
en_motor1 = 5
en_motor2 = 6
L1_motor = 22
L2_motor = 27
R1_motor = 17
R2_motor = 4

#bluetooth module GPIO setup
#need 3 GPIO pins and TX and RX pins

#general global variables
trig_delay=.00001
read_delay = .5

def sigterm_handler(signal,frame):
	GPIO.cleanup()
	print("cleaning up gpio from signal handler")
	exit(0)
signal.signal(signal.SIGINT, sigterm_handler)
signal.signal(signal.SIGTERM, sigterm_handler)

def ultrasonicInit():
	GPIO.output(trig, False)
	time.sleep(2)
	print("starting ultrasonic sensor")

def ultrasonicRead():
	GPIO.output(trig, True)
	time.sleep(trig_delay)
	GPIO.output(trig, False)
	while GPIO.input(echo) == 0:
		read_start = time.time()
	while GPIO.input(echo) == 1:
		read_end = time.time()
	read_duration = read_end - read_start
	distance = read_duration*17150
	print("distance in cm: ",distance)
	return distance

def IRread(one, two, three):
	# Read sensor input and print some diagnostics
	#read left sensor
	GPIO.setup(one, GPIO.OUT)
	GPIO.output(one, True)
	one_start = time.time()
	GPIO.setup(one, GPIO.IN)
	while GPIO.input(one) == 1:
		one_end = time.time()
	#read middle sensor
	GPIO.setup(two, GPIO.OUT)
	GPIO.output(two, True)
	two_start = time.time()
	GPIO.setup(two, GPIO.IN)
	while GPIO.input(two) == 1:
		two_end = time.time()
	#read right sensor
	GPIO.setup(three, GPIO.OUT)
	GPIO.output(three, True)
	three_start = time.time()
	GPIO.setup(three, GPIO.IN)
	while GPIO.input(three) == 1:
		three_end = time.time()
	one_pulse = (one_end-one_start)*1000
	two_pulse = (two_end-two_start)*1000
	three_pulse = (three_end-three_start)*1000
	return (one_pulse,two_pulse,three_pulse)
blockedCount = 0
freeCount = 0
rightCount = 0
leftCount = 0
side1Count = 0
side1Seen = 0
side2Count = 0
side2Seen = 0
side3Count = 0
spaceCounting = 0
spaceCounter = 0
ultrasonicInit()

	GPIO.output(moveLED, True)
	while True:
		dist = ultrasonicRead()
		print(dist)
		if(dist < 12):
			if (blockedCount >= 3 and dist <= 8):
				GPIO.output(moveLED, False)
				blockedCount = 0
				freeCount = 0
			else:
				blockedCount = blockedCount +1
		else:
			if (freeCount >= 3 and dist > 8):
				GPIO.output(moveLED, True)
				freeCount = 0
				blockedCount = 0
			else:
				freeCount = freeCount +1
		(L,M,R) = IRread(right_ir, middle_ir, left_ir)
		print(L, M, R)
		if(M < .2):
			if(L < .2):
				if(rightCount >=3):
					GPIO.output(rightLED, True)
					GPIO.output(leftLED, False)
					rightCount=0
					leftCount = 0
				else:
					rightCount= rightCount +1
			if(R < .2):
				if(leftCount >= 3):
					GPIO.output(leftLED, True)
					GPIO.output(rightLED, False)
					rightCount = 0
					leftCount = 0
				else:
					leftCount = leftCount+1
		else:
			GPIO.output(leftLED, False)
			GPIO.output(rightLED, False)
			rightCount = 0
			leftCount = 0
	#	(s1,s2,s3) = IRread(side1_ir, side2_ir, side3_ir)
	#	if(spaceCounting  == 1):
	#		if(spaceCounter > 100):
	#			spaceCounting = 0
	#			spaceCounter = 0
	#			side1Seen = 0
	#			side1Count = 0
	#			side2Seen = 0
	#			side2Count = 0
	#			side3Count = 0
	#		else:
	#			sideCounter=sideCounter+1
	#	if(s1 > .2):
	#		if(s2 > .2 and s3 > .2):
	#			##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
	#		else:
	#			if(side1Count >=2):
	#				spaceCounting = 1
	#				side1Seen = 1
	#				side1Count = 0
	#			else:
	#				side1Count  = side1Count +1
	#	if(s2 > .2):
	#		if(s1 > .2 and s3 > .2):
	#			 ##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
	#		else:
	#			if(side2Count >=2 and side1Seen == 1):
	#				side2Count = 0
	#				print("passed game space!")
	#				side1Seen = 0
	#				side2Seen = 0
	#				spaceCounting = 0
	#			elif(side2Count >=2):
	#				spaceCounting = 1
	#				side2Seen = 1
	#				side2Count = 0
	#			else:
	#				side2Count = side2Count +1
	#	if(s3 > .2):
	#		if(s1 > .2 and s2 > .2):
	#			##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
	#		elif(spaceCounting):
	#			if(side3Count >= 2 and (side1Seen == 1or side2Seen ==1)):
	#				side3Count = 0
	#				print("passed game space!")
	#				side1Seen = 0
	#				side2Seen = 0
	#				spaceCounting = 0
	#			else:
	#				side3Count = side2Count+1
		time.sleep(.2)
