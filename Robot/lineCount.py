import time
import RPi.GPIO as GPIO
import signal
from motors import *
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

#motor GPIOs
enA = 18
enB = 17
in1 = 6
in2 = 5
in3 = 23
in4 = 24
speed=25
lmotor=initMotor(enA,in1,in2,speed)
rmotor=initMotor(enB,in3,in4,speed)


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
	#print("distance in cm: ",distance)
	return distance

def IRread(one, two, three):
	# Read sensor input and print some diagnostics
	#read left sensor
	#fix this!!!!
	one_end = 0
	two_end = 0
	three_end = 0
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
	if(one_end == 0 or two_end == 0 or three_end == 0):
		return(-1,-1,-1)
	else:
		one_pulse = (one_end-one_start)*1000
		two_pulse = (two_end-two_start)*1000
		three_pulse = (three_end-three_start)*1000
		return (one_pulse,two_pulse,three_pulse)
speed = 25
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
forwardCount = 0
veerCounter = 0
leftVeer = 0
rightVeer = 0
ultrasonicInit()
sideCounter=0
startMotors(lmotor,rmotor)
moveForward(lmotor,rmotor,speed)
while(True):
	#dist = ultrasonicRead()
	dist = 20
	if(dist < 12):
		if (blockedCount >= 3 and dist <= 8):
			stopMoving(lmotor,rmotor)
			freeCount = 0
		else:
			blockedCount = blockedCount +1
	else:
		if (freeCount >= 3 and dist > 8 and blockedCount > 0):
			moveForward(lmotor,rmotor,speed)
			freeCount = 0
			blockedCount = 0
		else:
			freeCount = freeCount +1
	(L,M,R) = IRread(right_ir, middle_ir, left_ir)
	print(L, M, R)
	if(veerCounter > 8):
		veerCounter = 0
		if(leftVeer ==1):
			rightVeer = 1
			veerLeft(lmotor,rmotor,speed)
		elif(rightVeer == 1):
			leftVeer = 1
			veerRight(lmotor,rmotor,speed)
			leftVeer = 0
			rightVeer = 0
	if(M < .22):
		if(L > .25):
			if(leftCount >= 1):
				if(leftVeer ==1):
					veerCounter = veerCounter+1
				else:
					leftVeer =  1
					veerRight(lmotor,rmotor,speed)
					rightVeer = 0
					veerCounter = 0
					rightCount=0
					leftCount = 0
			else:
				leftCount= leftCount +1
		if(R > .2):
			if(rightCount >= 1):
				if(rightVeer == 1):
					veerCounter = veerCounter+1
				else:
					rightVeer = 1
					veerLeft(lmotor,rmotor,speed)
					leftVeer = 0
					veerCounter = 0
					rightCount = 0
					leftCount = 0
			else:
				rightCount = rightCount+1
	else:
		forwardCount = forwardCount +1
		if(forwardCount >= 5):
			moveForward(lmotor,rmotor,speed)
			rightCount = 0
			leftCount = 0
			forwardCount = 0
	(s1,s2,s3) = IRread(side1_ir, side2_ir, side3_ir)
	#print(s1,s2,s
	if(spaceCounting  == 1):
		if(spaceCounter > 100):
			spaceCounting = 0
			spaceCounter = 0
			side1Seen = 0
			side1Count = 0
			side2Seen = 0
			side2Count = 0
			side3Count = 0
		else:
			sideCounter=sideCounter+1
	if(s1 > .2):
		if(s2 > .2 and s3 > .2):
			##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
			pass
		else:
			if(side1Count >=2):
				spaceCounting = 1
				side1Seen = 1
				side1Count = 0
			else:
				side1Count  = side1Count +1
	if(s2 > .2):
		if(s1 > .2 and s3 > .2):
			 ##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
			 pass
		elif(spaceCounting):
			spaceCounter = spaceCounter +1
			if(side2Count >=2 and side1Seen == 1):
				side2Count = 0
				print("passed game space1!")
				side1Seen = 0
				side2Seen = 0
				spaceCounting = 0
				spaceCount = 0
			elif(side2Count >=2):
				spaceCounting = 1
				side2Seen = 1
				side2Count = 0
			else:
				side2Count = side2Count +1
	if(s3 > .2):
		if(s1 > .2 and s2 > .2):
			##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
			pass
		elif(spaceCounting):
			spaceCounter = spaceCounter+1
			if(side3Count >= 2 and (side1Seen == 1or side2Seen ==1)):
				side3Count = 0
				print("passed game space2!")
				side1Seen = 0
				side2Seen = 0
				spaceCounting = 0
				spaceCounter = 0
			else:
				side3Count = side2Count+1
	time.sleep(.2)
