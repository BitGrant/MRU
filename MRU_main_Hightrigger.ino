/*
  Basic Arduino example for K-Series sensor
  Created by Jason Berger
  Co2meter.com  
*/
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SoftwareSerial.h>


#define ONE_WIRE_BUS 5
#define SensorPin1 A0 
#define SensorPin2 A1 
#define SensorPin3 A2 
//#define SensorPin4 A3 


OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
SoftwareSerial K_30_Serial(12,13);  //Sets up a virtual serial port
                                    //Using pin 12 for Rx and pin 13 for Tx

 float Celcius=0;
 float Fahrenheit=0;
 float sensorValue1 = 0;
 float sensorValue2 = 0; 
 float sensorValue3 = 0; 
 float sensorValue4 = 0;  
 
byte readCO2[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};  //Command packet to read Co2 (see app note)
byte response[] = {0,0,0,0,0,0,0};  //create an array to store the response

//multiplier for value. default is 1. set to 3 for K-30 3% and 10 for K-33 ICB
int valMultiplier = 1;
int Trigger = 9;
int TriggerState;

void setup() 
{
  // put your setup code here, to run once:
  pinMode(Trigger, INPUT);
  Serial.begin(9600);//Opens the main serial port to communicate with the computer
  
  K_30_Serial.begin(9600);    //Opens the virtual serial port with a baud of 9600
  sensors.begin();
}
//////////// CO2 Sensor Code ///////////////////////////////////////
//void sendRequest(byte packet[])
//{
//  while(!K_30_Serial.available())  //keep sending request until we start to get a response
//  {
//    K_30_Serial.write(readCO2,7);
//    delay(50);
//  }
//  
//  int timeout=0;  //set a timeoute counter
//  while(K_30_Serial.available() < 7 ) //Wait to get a 7 byte response
//  {
//    timeout++;  
//    if(timeout > 10)    //if it takes to long there was probably an error
//      {
//        while(K_30_Serial.available())  //flush whatever we have
//          K_30_Serial.read();
//          
//          break;                        //exit and try again
//      }
//      delay(50);
//  }
//  
//  for (int i=0; i < 7; i++)
//  {
//    response[i] = K_30_Serial.read();
//  }  
//}
//
//unsigned long getValue(byte packet[])
//{
//    int high = packet[3];                        //high byte for value is 4th byte in packet in the packet
//    int low = packet[4];                         //low byte for value is 5th byte in the packet
//
//  
//    unsigned long val = high*256 + low;                //Combine high byte and low byte with this formula to get value
//    return val* valMultiplier;
//}
//


// LOOP begins//////////////////////////////////////////////////
void loop() { 
 
  TriggerState = digitalRead(Trigger);
  if (TriggerState == HIGH)
{
  sendRequest(readCO2);
  unsigned long valCO2 = getValue(response);
//  Serial.print("Co2 ppm = ");
//  Serial.print(valCO2);
  sensorValue1 = analogRead(SensorPin1);
  sensorValue2 = analogRead(SensorPin2);
  sensorValue3 = analogRead(SensorPin3);
//  sensorValue4 = sensorValue4 + analogRead(SensorPin4);
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius); 
  Serial.print(valCO2);
  Serial.print(",");
  Serial.print(Celcius);
  Serial.print(",");
//  Serial.print(" F  ");
  Serial.print(Fahrenheit);
  Serial.print(",");
//  Serial.print(" MC ");
//  sensorValue = sensorValue/100.0; 
  Serial.print(sensorValue1);
  Serial.print(",");
  Serial.print(sensorValue2);
  Serial.print(",");
  Serial.println(sensorValue3);
//  Serial.println(sensorValue1);
//  Serial.println(sensorValue1);
  delay(2000);
  


}
}

void sendRequest(byte packet[])
{
  while(!K_30_Serial.available())  //keep sending request until we start to get a response
  {
    K_30_Serial.write(readCO2,7);
    delay(50);
  }
  
  int timeout=0;  //set a timeoute counter
  while(K_30_Serial.available() < 7 ) //Wait to get a 7 byte response
  {
    timeout++;  
    if(timeout > 10)    //if it takes to long there was probably an error
      {
        while(K_30_Serial.available())  //flush whatever we have
          K_30_Serial.read();
          
          break;                        //exit and try again
      }
      delay(50);
  }
  
  for (int i=0; i < 7; i++)
  {
    response[i] = K_30_Serial.read();
  }  
}

unsigned long getValue(byte packet[])
{
    int high = packet[3];                        //high byte for value is 4th byte in packet in the packet
    int low = packet[4];                         //low byte for value is 5th byte in the packet

  
    unsigned long val = high*256 + low;                //Combine high byte and low byte with this formula to get value
    return val* valMultiplier;
}
