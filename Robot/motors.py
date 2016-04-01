import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

def setSpeed(motor, speed):
   motor.ChangeDutyCycle(speed)

def stopMotor(motor):
   motor.stop()

def startMotor(motor,speed):
   motor.start(speed)

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

