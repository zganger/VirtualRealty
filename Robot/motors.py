import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)

def setSpeed(motor, speed):
   motor.ChangeDutyCycle(speed)

def stopMotors(lmotor,rmotor):
   lmotor.stop()
   rmotor.stop()

def startMotors(lmotor,rmotor):
   lmotor.start(0)
   rmotor.start(0)

def initMotor(pwm,in1,in2, freq):
   #freq in Hz
   GPIO.setup(in1,GPIO.OUT)
   GPIO.setup(in2,GPIO.OUT)
   GPIO.setup(pwm,GPIO.OUT)
   #GPIO.output(in1,GPIO.HIGH) #forward
   #GPIO.output(in2,GPIO.LOW)
   wheelForward(in1,in2)
   return GPIO.PWM(pwm,freq)

def setFrequency(motor,freq):
   motor.ChangeFrequency(freq)

def turnLeft(lmotor,rmotor,speed):
   setSpeed(lmotor,0)
   setSpeed(rmotor,speed)
#   print("Left")

def turnRight(lmotor,rmotor,speed):
   setSpeed(lmotor,speed)
   setSpeed(rmotor,0)
#   print("Right")

def moveForward(lmotor,rmotor,speed):
   setSpeed(rmotor,speed)
   setSpeed(lmotor,speed)
#   print("Forward")

def veerLeft(lmotor,rmotor,speed):
	setSpeed(lmotor,speed*(80/100))
	setSpeed(rmotor,speed)
#	print("VeerL")

def veerRight(lmotor,rmotor,speed):
	setSpeed(rmotor,speed*(80/100))
	setSpeed(lmotor,speed)
#	print("VeerR")

def hardVeerRight(lmotor,rmotor,speed):
	setSpeed(rmotor,speed*(75/100))
	setSpeed(lmotor,speed)

def hardVeerLeft(lmotor,rmotor,speed):
	setSpeed(lmotor,speed*(75/100))
	setSpeed(rmotor,speed)

def turnVeerLeft(lmotor,rmotor,speed):
        setSpeed(lmotor,speed*(65/100))
        setSpeed(rmotor,speed)

def stopMoving(lmotor,rmotor):
	setSpeed(lmotor,0)
	setSpeed(rmotor,0)

def inPlaceRight(lmotor,rmotor,speed,in1,in2):
	setSpeed(lmotor,0)
	setSpeed(rmotor,speed)
	wheelBackwards(in1,in2)

def fullBackwards(lmotor,rmotor,speed):
        setSpeed(lmotor,0)
        setSpeed(rmotor,speed)


def fullForward(lmotor,rmotor,speed):
        setSpeed(lmotor,speed)
        setSpeed(rmotor,0)

def wheelForward(in1,in2):
	GPIO.output(in1,GPIO.HIGH)
	GPIO.output(in2,GPIO.LOW)

def wheelBackwards(in1,in2):
	GPIO.output(in1,GPIO.LOW)
	GPIO.output(in2,GPIO.HIGH)
