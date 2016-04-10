import RPi.GPIO as GPIO
import serial
from time import sleep
import signal
import struct

def sigterm_handler(signal,frame):
	GPIO.cleanup()
	print("cleaning up gpio from signal handler")
	exit(0)
signal.signal(signal.SIGINT, sigterm_handler)
signal.signal(signal.SIGTERM, sigterm_handler)

BLUEFRUIT_MODE_COMMAND = 1
BLUEFRUIT_MODE_DATA = 0
CTS = 16 #23
RTS = 17 #24
mode_pin = 22
GPIO.setmode(GPIO.BCM)
GPIO.setup(CTS, GPIO.OUT)
GPIO.setup(RTS, GPIO.IN)
GPIO.setup(mode_pin, GPIO.OUT)
GPIO.output(mode_pin,GPIO.HIGH)

ser=0
rxText=""
connect=0
mode = BLUEFRUIT_MODE_COMMAND
def reset():
	#isOK=False
	#return T/F
	#while(not isOK):
	#	isOK=sendCommandCheckOK("ATE=0")
	for i in range(5):
		#print("->ATZ")
		isOK = sendCommandCheckOK("ATZ")
		if(isOK):
			break
			
	if(not isOK):
		sleep(.05)
		setMode(BLUEFRUIT_MODE_COMMAND)
		sleep(.05)
		
		for i in range(5):
			isOK = sendCommandCheckOK("ATZ")
			if(isOK):
				break
		if(not isOK):
			return False
	#Bluefruit need 1 second to reboot
	sleep(.001)
	
	flush()
	
	return isOK

def factoryReset():
	#return T/F
	println("AT+FACTORYRESET")
	isOK = waitForOK()
	#Bluefruit need 1 second to reboot
	sleep(.001)
	
	flush()
	
	return isOK

def waitForOK():
	#return T/F
	global connect,rxText
	counter=0
	while(1):
		ret = readLine()
#		print(ret)
		if(ret):
			#print("RET", repr(ret))
			#print("here")
			ret=ret.split("\n")
			#print(len(ret))
			if(len(ret)==4):
				if(ret[1]=="0\r" or ret[1]=="1\r"):
					connect=int(ret[1].rstrip("\r"))
			#		print("Conn",connect)
				else:
					rxText=ret[1].rstrip("\r")
			ret=ret[len(ret)-2]+"\n"
#			print(ret)
		counter+=1
		if(ret=="OK\r\n"):
			return True
		if(ret=="ERROR\r\n" or counter == 500):
			return False

def isConnected():
	#return T/F
	return sendCommandWithIntReply("AT+GAPGETCONN") 

def disconnect():
	sendCommandCheckOK("AT+GAPDISCONNECT")


def sendCommandCheckOK(cmd):
	#return T/F
	global mode
	current_mode = mode;
	if(current_mode == BLUEFRUIT_MODE_DATA):
		setMode(BLUEFRUIT_MODE_COMMAND)
	#print(cmd)
	println(cmd)
	#print("waitingForOK...")
	result = waitForOK()
	#print(result)
	if(current_mode == BLUEFRUIT_MODE_DATA):
		setMode(BLUEFRUIT_MODE_DATA)
	return result
	

def sendCommandWithIntReply(cmd):
#return T/F or num??
	#print("intRep")
	current_mode = mode;
	if(current_mode == BLUEFRUIT_MODE_DATA):
		setMode(BLUEFRUIT_MODE_COMMAND)
	println(cmd)
#	print("waitingForOK...Int")
	result = waitForOK()
	result = connect;
	if(current_mode == BLUEFRUIT_MODE_DATA):
		setMode(BLUEFRUIT_MODE_DATA)
	#ret=readLine()
	#print(result)
	return result


def readLine():
	c=None
	while(available()>0):
		#print("available")
		try:
			c=ser.read(100)
			#print("read: ",c)
			c=c.decode('utf-8')
		except UnicodeDecodeError:
			#print(struct.unpack("<bbb",c))
			c=None
			continue
			
		#print(repr(c))
#		if(len(c)>0):
#			print(c)
	return c
	
def begin():
	global ser, CTS
	ser = serial.Serial('/dev/ttyAMA0',9600,timeout=.250,rtscts=1)
	GPIO.output(CTS,GPIO.HIGH)
	
	return reset()
	
def end():
	global ser
	ser.close()
	
def setMode(new_mode):
	global mode_pin
	GPIO.output(mode_pin,new_mode)
	sleep(.001)
	isOK = True
	return isOK
	
def write(c):
	global ser, RTS
	while(GPIO.input(RTS)):
		sleep(.001)
	sleep(50/1000000.0)
	ser.write(c.encode('utf-8'))
	
def available():
	global ser
	if(ser.inWaiting()<1):
		GPIO.output(CTS,GPIO.LOW)
		sleep(.001)
		GPIO.output(CTS,GPIO.HIGH)
	
	return ser.inWaiting()
	
def read():
	global ser
	return ser.read()
	
def flush():
	global ser
	ser.flush()

def println(cmd):
	for i in cmd:
		write(i)
	write("\r")
	write("\n")
	
