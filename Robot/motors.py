import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

def setSpeed(motor, speed):
   motor.ChangeDutyCycle(speed)

def stopMotors(lmotor,rmotor):
   lmotor.stop()
   rmotor.stop()

def startMotors(lmotor,rmotor,speed):
   lmotor.start(speed)
   rmotor.start(speed)

def initMotor(pwm,in1,in2, freq):
   #freq in Hz
   GPIO.setup(in1,GPIO.OUT)
   GPIO.setup(in2,GPIO.OUT)
   GPIO.setup(pwm,GPIO.OUT)
   GPIO.output(in1,GPIO.HIGH) #forward
   GPIO.output(in2,GPIO.LOW)
   return GPIO.PWM(pwm,freq)

def setFrequency(motor,freq):
   motor.ChangeFrequency(freq)

def turnLeft(lmotor,rmotor,speed):
   lmotor.setSpeed(0)
   rmotor.setSpeed(speed)

def turnRight(lmotor,rmotor,speed):
   lmotor.setSpeed(speed)
   rmotor.setSpeed(0)

def moveForward(lmotor,rmotor,speed):
   rmotor.setSpeed(speed)
   lmotor.setSpeed(speed)


