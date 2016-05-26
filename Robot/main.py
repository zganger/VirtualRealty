import time
import RPi.GPIO as GPIO
import signal
from motors import *
GPIO.setmode(GPIO.BCM)
import BLE as ble
#setup GPIOs for ultrasonic sensor
trig = 12
echo = 25
GPIO.setup(trig, GPIO.OUT)
GPIO.setup(echo, GPIO.IN)

#setup GPIO for line following IR sensors
right_ir =16
middle_ir =20
left_ir =21
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

#motor GPIOs
enA = 18
enB = 17
in1 = 6
in2 = 5
in3 = 23
in4 = 24
speed=40
lmotor=initMotor(enA,in1,in2,speed)
rmotor=initMotor(enB,in3,in4,speed)

led=10
GPIO.setup(led,GPIO.OUT)

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
	read_start = 0
	read_end = 0
	GPIO.output(trig, True)
	time.sleep(trig_delay)
	GPIO.output(trig, False)
	while GPIO.input(echo) == 0:
		read_start = time.time()
	while GPIO.input(echo) == 1:
		read_end = time.time()
	read_duration = read_end - read_start
	distance = read_duration*17150
#	print("distance in cm: ",distance)
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

def initBluetooth():
	print("Beginning...")
	ble.begin()
	print("Resetting...")
	ble.factoryReset()

def decodeBluetooth(n):
        return(int(n[0]),int(n[1:3]),int(n[3:]))
def getBluetoothStr(wait):
        if(wait):
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
        else:
                  ble.println("AT+BLEUARTRX")
                  rx=ble.readLine()
                        #print(rx)
                        #print(ble.rxText)
    #                    if(rx=="OK\r\n"):
   #                     #        continue
                  #if(ble.rxText!=""):
                   #  print(ble.rxText)
                          #      ble.rxText=""
                         #       break
                  ble.waitForOK()

        rxVal=ble.rxText
        ble.rxText=''
        return rxVal

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
atDest = 0
stopped = 0
position = 0
stable = 0
onCorner =0
breakcount = 0
stableCount = 0
sideCounter = 0
findTurn = 0
turning = 0
turnNum = 0
switchCount = 0
stopTurnCount = 0
turnDelay = 100
ultrasonicInit()
initBluetooth()
commandList=[]
commandIndex=-1
while(True):
	#print("here")
	if(commandIndex==(len(commandList)-1)):
		command=getBluetoothStr(True)
		commandList.append(command)
	commandIndex+=1
	print(commandList[commandIndex])
	position = 0
	dNum,dest,corner=decodeBluetooth(commandList[commandIndex])
	dNum,dest,corner=decodeBluetooth(command)
	if onCorner == 1:
		onCorner = 0
		findTrun = 1
	if dest == corner:
		onCorner = 1
	startMotors(lmotor,rmotor)
	moveForward(lmotor,rmotor,speed)
	while(True):
		newcommand=getBluetoothStr(False)
		if(newcommand!=''):
			print(newcommand, "added to list")
			commandList.append(newcommand)
		if(position ==dest):
			atDest = 1
		if(position == corner):
			findTurn = 1
			if(dest > corner+10):
				corner = corner+10
			else:
				corner = 0
		dist = ultrasonicRead()
		if(dist < 8):
			stopMoving(lmotor,rmotor)
			stopped = 1
		else:
			stopped = 0
		if(stopped != 1 and turning !=1 and findTurn != 1):
			if(atDest == 1):
				breakcount = breakcount +1
				if(breakcount > 150):
					atDest = 0
					breakcount = 0
					stopMoving(lmotor,rmotor)
					break;

			(L,M,R) = IRread(right_ir, middle_ir, left_ir)
			#print(L, M, R)
			if(L < .2 and R < .2 and M < .2):
				if(findTurn):
					stopMocing(lmotor,rmotor)
					wheelForward(in1,in2)
					wheelBackwards(in3,in4)
					findTurn = 0
					turning = 1
					turnNum = 0
				else:
					veerCounter = veerCounter +1
					if(veerCounter > 10000):
#						print("change veer")
						veerCounter = 0
						if(leftVeer ==1):
							rightVeer = 1
							leftVeer = 0
							veerRight(lmotor,rmotor,speed)
						elif(rightVeer == 1):
							leftVeer = 1
							veerLeft(lmotor,rmotor,speed)
							rightVeer = 0
			if(M < .3):
				if(L > .3):
					#print("veerLeft")
					leftVeer =  1
					hardVeerLeft(lmotor,rmotor,speed)
					rightVeer = 0
				elif(R > .3):
					#print("veerRight")
					rightVeer = 1
					hardVeerRight(lmotor,rmotor,speed)
					leftVeer = 0
			else:
				if(L>.3):
					veerRight(lmotor,rmotor,speed)
					rightVeer =1
					leftVeer = 0
				if(R>.3):
					veerLeft(lmotor,rmotor,speed)
					leftVeer = 1
					rightVeer = 0
				else:
					if(veerCounter >0):
						if(rightVeer ==1):
							veerLeft(lmotor,rmotor,speed)
							stableCount = stableCount+1
						else:
							veerRight(lmotor,rmotor,speed)
							stableCount = stableCount +1
						if(stableCount > 400):
							stable = 1
					if(stable == 1):
#						print("forward")
						moveForward(lmotor,rmotor,speed)
						veerCounter = 0
			(s1,s2,s3) = IRread(side1_ir, side2_ir, side3_ir)
	#		print(s1,s2,s3)
			if(spaceCounting  == 1):
				if(spaceCounter > 3000):
#					print("reset")
					spaceCounting = 0
					spaceCounter = 0
					side1Seen = 0
					side1Count = 0
					side2Count = 0
					side2Seen = 0
				else:
					spaceCounter=spaceCounter+1
			if(s1 > .3):
				if(s2 < .3 and s3 < .3):
					if(side1Count >=2):
						spaceCounting = 1
						side1Seen = 1
						side1Count = 0
					else:
						side1Count  = side1Count+1
			if(s2 > .3):
				if(spaceCounting and s1 < .3 and s3 < .3):
#					print("Space",spaceCounting)
					if(side2Count >=2 and side1Seen == 1):
						side2Count = 0
						print("passed game space1!")
						position = position +1
						side1Seen = 0
						side2Seen = 0
						spaceCounting = 0
						spaceCounter = 0
					elif(side2Count >=2):
						spaceCounting = 1
						side2Seen = 1
						side2Count = 0
					else:
						side2Count = side2Count +1
			if(s3 > .3):
				if(spaceCounting and s1 < .3 and s2 < .3):
					if(side3Count >= 2 and (side1Seen == 1 or side2Seen ==1)):
						side3Count = 0
						print("passed game space2!")
						position = position +1
						side1Seen = 0
						side2Seen = 0
						spaceCounting = 0
						spaceCounter = 0
					else:
						side3Count = side2Count+1
		elif turning == 1:
			(L,M,R) = IRread(right_ir, middle_ir, left_ir)
			if L < .3:
				if turnNum%2 ==0 and M< .3:
					fullBackwards(lmotor,rmotor,speed)
				elif turnNum%2 == 1 and M>.3:
					fullForward(lmotor,rmotor,speed)
				else:
					switchCount = switchCount +1
					if(switchCount > 3):
						switchCount = 0
						turnNum = turnNum +1
			else:
				stopTurnCount = stopTurnCount +1
				if(stopTurnCount >3):
					wheelForward(in3,in4)
					turnVeerLeft(lmotor,rmotor,speed)
					turnNum = 0
					turning =0
					stopTurnCount = 0
					switchCount = 0
		elif findTurn == 1:
			(L,M,R) = IRread(right_ir, middle_ir, left_ir)
			if(L < .15 and R < .15 and M < .15):
				stopMoving(lmotor,rmotor)
				wheelForward(in1,in2)
				wheelBackwards(in3,in4)
				findTurn = 0
				turning = 1



