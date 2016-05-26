import time
import RPi.GPIO as GPIO
import signal
from motors import *
import BLE as ble
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
"""rightLED = 18
leftLED = 23
moveLED = 24
GPIO.setup(rightLED, GPIO.OUT)
GPIO.setup(leftLED, GPIO.OUT)
GPIO.setup(moveLED, GPIO.OUT)
"""
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
TRIG_DELAY=.00001
SETUP_DELAY = 2
DIST_THRESHOLD = 10
DIST_COUNT = 2
LINE_THRES = .3
M_LINE_THRES = .2
LINE_COUNT = 2
SPACE_COUNT = 100
SIDE_COUNT = 2
VEER_COUNT = 5
STOP_DELAY = .5
SPEED = 25
TURN_WAIT = .5
TURN_DELAY = .5
HALF_TURN_DEALY = TURN_DELAY/2
def sigterm_handler(signal,frame):
	GPIO.cleanup()
	print("cleaning up gpio from signal handler")
	exit(0)
signal.signal(signal.SIGINT, sigterm_handler)
signal.signal(signal.SIGTERM, sigterm_handler)

def ultrasonicInit():
	global SETUP_DELAY
	GPIO.output(trig, False)
	time.sleep(SETUP_DELAY)
	print("starting ultrasonic sensor")

def ultrasonicRead():
	global TRIG_DELAY
	GPIO.output(trig, True)
	time.sleep(TRIG_DELAY)
	GPIO.output(trig, False)
	while GPIO.input(echo) == 0:
		read_start = time.time()
	while GPIO.input(echo) == 1:
		read_end = time.time()
	read_duration = read_end - read_start
	distance = read_duration*17150
	#print("distance in cm: ",distance)
	return distance

def initBluetooth():
        print("Beginning...")
        ble.begin()
        print("Resetting...")
        ble.factoryReset()

def decodeBluetooth(n):
        return(int(n[0]),int(n[1:3]),int(n[3:]))

def getBluetoothStr():
        print("Waiting for Commands...")
        while(True):
                ble.println("AT+BLEUARTRX")
                rx=ble.readLine()
                #print(rx)
                #print(ble.rxText)
                if(rx=="OK\r\n"):
                        continue
                if(ble.rxText!=""):
                        #print(ble.rxText)
                        ble.rx=""
                        break
                ble.waitForOK()

        return ble.rxText

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
sideCounter = 0
leftVeer = 0
rightVeer = 0
veerCounter = 0
onCorner = 0
beforeCorner = 0
afterCorner = 0
ultrasonicInit()
string = ""
startMotors(lmotor,rmotor)
while(True):
	global TURN_WAIT,TURN_DELAY,SPEED,DIST_COUNT,DIST_THRESHOLD,VEER_COUNT,LINE_COUNT,LINE_THRES,SIDE_COUNT,SPACE_COUNT,STOP_DELAY
	while(string == ""):
		string = getBluetoothStr()
	#idNum = int(string[0])
	#dest = int(string[1]+string[2])
	#corner = int(string[3]+string[4])
	idNum,dest,corner=decodeBluetooth(string)
	print("id",idNum)
	print("Dest:",dest)
	print("corner",corner)
	position = 0
	if(beforeCorner == 1):
		if(dest == corner):
			turnRight(lmotor,rmotor,SPEED)
			time.delay(HALF_TURN_DELAY)
			stopMoving(lmotor,rmotor)
			position = dest
			beforeCorner = 0
			onCorner = 1
		else:
			turnRight(lmotor,rmotor,SPEED)
			time.delay(TURN_DELAY)
			stopMoving(lmotor,rmotor)
			position = position + 2
	elif(onCorner == 1):
		turnRight(lmotor,rmotor,SPEED)
		time.delay(HALF_TURN_DELAY)
		stopMotors(lmotor,rmotor,SPEED)
		onCorner = 0
		if(idNum == 0):
			position = position +1
	if(dest == corner):
		onCorner = 1
		if(idNum == 0):
			corner = corner -1
			dest = dest -1
	if(idNum == 0):
		if(dest > (corner+1)):
			corner = corner -1
			dest = dest -2
		elif(dest == corner -1):
			beforeCorner = 1
		elif(dest == corner +1):
			afterCorner = 1
			corner = corner -1
			dest = dest -2
		inside_ir = left_ir
		outside_ir = right_ir
	elif(idNum == 1):
		inside_ir = right_ir
		outside_ir = left_ir
	while(position != dest):
		if (position == corner):
			print("turn corner!")
			time.sleep(TURN_WAIT)
			#turn_corner(); #this should set a time delay in the beginning, and then start motors, then stop motors after another time delay
			time.sleep(TURN_DELAY)
			moveForward(lmotor,rmotor,SPEED)
			if((dest-position)< (corner+10)):
				corner = corner+10
		dist = ultrasonicRead()
		print("Position:",position)
		#print(dist)
		if(dist < DIST_THRESHOLD):
			if (blockedCount >= DIST_COUNT and dist <= 8):
				#GPIO.output(moveLED, False)
				stopMotors(lmotor,rmotor)
				blockedCount = 0
				freeCount = 0
			else:
				blockedCount = blockedCount +1
		else:
			if (freeCount >= DIST_COUNT and dist > 8):
				#GPIO.output(moveLED, True)
				startMotors(lmotor,rmotor,SPEED)
				freeCount = 0
				blockedCount = 0
			else:
				freeCount = freeCount +1
		(O,M,I) = IRread(outside_ir, middle_ir, inside_ir)
		print(O, M, I)
		if(veerCounter > VEER_COUNT):
			veerCounter = 0
			if(leftVeer ==1):
				rightVeer = 1
				veerRight(lmotor,rmotor,SPEED)
			elif(rightVeer == 1):
				leftVeer = 1
				veerLeft(lmotor,rmotor,SPEED)
			leftVeer = 0
			rightVeer = 0
		if(M < M_LINE_THRES):
			if(O > LINE_THRES):
				if(leftCount >=LINE_COUNT):
#					GPIO.output(rightLED, True)
#					GPIO.output(leftLED, False)
					if(leftVeer == 1):
						veerCounter = veerCount +1
					else:
						leftVeer = 1
						veerLeft(lmotor,rmotor,SPEED)
						rightVeer = 0
						veerCounter = 0
						rightCount=0
						leftCount = 0
				else:
					leftCount= leftCount +1
			elif(I > LINE_THRES):
				if(rightCount >= LINE_COUNT):
					#GPIO.output(leftLED, True)
					#GPIO.output(rightLED, False)
					if(rightVeer == 1):
						veerCounter = veerCounter +1
					else:
						rightVeer = 1
						veerRight(lmotor,rmotor,SPEED)
						veerCounter = 0
						leftVeer = 0
						rightCount = 0
						leftCount = 0
				else:
					rightCount = rightCount+1
		else:
			#GPIO.output(leftLED, False)
			#GPIO.output(rightLED, False)
			leftVeer = 0
			rightVeer = 0
			veerCounter = 0
			moveForward(lmotor,rmotor,SPEED)
			rightCount = 0
			leftCount = 0
		(s1,s2,s3) = IRread(side1_ir, side2_ir, side3_ir)
#		print(s1,s2,s3)
		if(spaceCounting  == 1):
			if(spaceCounter > SPACE_COUNT):
				spaceCounting = 0
				spaceCounter = 0
				side1Seen = 0
				side1Count = 0
		if(s1 > LINE_THRES):
			if(s2 > LINE_THRES and s3 > LINE_THRES):
				##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
				pass
			else:
				if(side1Count >=SIDE_COUNT):
					spaceCounting = 1
					side1Seen = 1
					side1Count = 0
				else:
					side1Count  = side1Count +1
		if(s2 > LINE_THRES):
			if(s1 > LINE_THRES and s3 > LINE_THRES):
       				##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
				pass
			elif(spaceCounting):
				spaceCounter = spaceCounter +1
				if(side2Count >=SIDE_COUNT and side1Seen == 1):
					side2Count = 0
					print("passed game space1!")
					side1Seen = 0
					side2Seen = 0
					spaceCounting = 0
					spaceCount = 0
				elif(side2Count >=SIDE_COUNT):
					spaceCounting = 1
					side2Seen = 1
					side2Count = 0
				else:
					side2Count = side2Count +1
		if(s3 > LINE_THRES):
			if(s1 > LINE_THRES and s2 > LINE_THRES):
				##this means that we are probably too far  to the side or ir sensor readings not calibrated - ignore?
				pass
			elif(spaceCounting):
				spaceCounter = spaceCounter+1
				if(side3Count >= SIDE_COUNT and (side1Seen == 1or side2Seen ==1)):
					side3Count = 0
					print("passed game space2!")
					side1Seen = 0
					side2Seen = 0
					spaceCounting = 0
					spaceCounter = 0
				else:
					side3Count = side2Count+1
	if(afterCorner == 1):
		time.sleep(TURN_WAIT)
		turnRight(lmotor,rmotor,speed)
		time.sleep(TURN_DELAY)
		stopMoving(lmotor,rmotor)
		afterCorner = 0
	elif(onCorner == 1):
		time.sleep(TURN_WAIT)
		turnRight(lmotor,rmotor,speed)
		time.sleep(HALF_TURN_DELAY)
		stopMoving(lmotor,rmotor)
	else:
		time.sleep(STOP_DELAY)
		stopMoving(lmotor,rmotor)
	print("reached destination!")
