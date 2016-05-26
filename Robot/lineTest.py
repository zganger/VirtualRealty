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
right_ir = 16 #21
middle_ir =20
left_ir = 21  #16
GPIO.setup(right_ir, GPIO.IN)
GPIO.setup(middle_ir, GPIO.IN)
GPIO.setup(left_ir, GPIO.IN)

#setup GPIO for space counting IR sensors
side1_ir =13
side2_ir =19
side3_ir =26
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
speed = 13
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
	(L,M,R) = IRread(right_ir, middle_ir, left_ir)
	print(L, M, R)
	if(L < .2 and R < .2 and M < .15):
		veerCounter = veerCounter +1
		if(veerCounter > 5000):
			print("change veer")
			veerCounter = 0
			if(leftVeer ==1):
				rightVeer = 1
				leftVeer = 0
				hardVeerRight(lmotor,rmotor,speed)
			elif(rightVeer == 1):
				leftVeer = 1
				hardVeerLeft(lmotor,rmotor,speed)
				rightVeer = 0
	elif(M < .2):
		if(L > .3):
			print("veerLeft")
			veerCounter = veerCounter +1
			leftVeer =  1
			veerLeft(lmotor,rmotor,speed)
			rightVeer = 0
		if(R > .3):
			print("veerRight")
			veerCounter = veerCounter+1
			rightVeer = 1
			veerRight(lmotor,rmotor,speed)
			leftVeer = 0
	else:
		print("forward")
		forwardCount = forwardCount +1
		if(forwardCount >= 3):
			moveForward(lmotor,rmotor,speed)
			veerCounter = 0
			forwardCount = 0
time.sleep(.05)
