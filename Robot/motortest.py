import RPi.GPIO as GPIO
import time
from motors import *
enA = 18
enB = 17
in1 = 6
in2 = 5
in3 = 23
in4 = 24

GPIO.setmode(GPIO.BCM)

rmotor=initMotor(enA,in1,in2,50)
lmotor=initMotor(enB,in3,in4,50)

startMotor(rmotor,50)
startMotor(lmotor,50)
time.sleep(5)
stopMotor(rmotor)
stopMotor(lmotor)

GPIO.cleanup()
