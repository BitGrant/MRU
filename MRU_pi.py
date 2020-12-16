#!/usr/bin/env python3

from __future__ import print_function
import datetime
from random import randint
from time import sleep
import Adafruit_DHT
import pickle
import os.path
import board
import busio
import socket
import adafruit_sgp30
import picamera
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)
import csv
import os.path
import os
import time
import serial
import time

DHT_SENSOR = Adafruit_DHT.DHT22
DHT_PIN = 4

GPIO.setup(15, GPIO.OUT)
GPIO.output(15, GPIO.HIGH)

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    ser.flush()
    
    while True:
        try:
            Time = '{:%D %H:%M}'.format(datetime.datetime.today())
            ser.flush()
        #Serial Components
#             ser.flush()
            GPIO.output(15, GPIO.HIGH)
#             ser.write(b"RequestFullData") #Sending request to Arduino- Doesn't matter what it says
            line = ser.readline().decode('utf-8').rstrip() #Recieve the data in line
            f = line.replace("'","") #Manipulate data
            f = f.split(',')
#             print(f)
            f2 = [float(i) for i in f] # Turn Data into a vector
            
        #Local Digital Components
            humidity, temperature = Adafruit_DHT.read_retry(DHT_SENSOR, DHT_PIN)
            
        #Data Organized
            AirTemp= round(temperature,1)
            AirHumid= round(humidity,1)
            valco2 = f2[0]
            SoiltempC = f2[1]
            SoiltempF = f2[2]
            SoilMoisture1= f2[3]
            SoilMoisture2= f2[4]
            SoilMoisture3= f2[5]
            
            print(AirTemp,AirHumid,valco2,SoiltempC,SoilMoisture1,SoilMoisture2,SoilMoisture3)
        
#Save Enviroment Data ##
    
            with open("/home/pi/MRUData.csv", "a") as log:
                log.write("{0},{1},{2},{3},{4},{5},{6},{7}\n".format(str(Time),str(AirTemp),str(AirHumid),str(valco2),str(SoiltempC),str(SoilMoisture1),str(SoilMoisture2),str(SoilMoisture3)))        
#         print(SoilMoisture1,SoiltempF,SoiltempC)
        
#Loop time
            GPIO.output(15, GPIO.LOW)
            time.sleep(10)
        except (ValueError, IndexError):
            print("Unexpected error:")
            time.sleep(5)