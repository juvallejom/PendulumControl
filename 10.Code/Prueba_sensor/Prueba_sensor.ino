//
//    FILE: AS5600_demo.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//    DATE: 2022-05-28


#include "AS5600.h"
#include "Wire.h"
int readPosition;
float realPosition;
int auxPosition;
int offset=3790;

//Pines Motor 

int ENA = 6; //Puente H enable
int IN1 = 8; //Puente H IN3
int IN2 = 9; //Puente H IN4 pwm

float pwmPercent;
int pwmDuty;

AS5600 as5600;   //  use default Wire


void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AS5600_LIB_VERSION: ");
  Serial.println(AS5600_LIB_VERSION);

  //  ESP32
  //  as5600.begin(14,15);
  //  AVR
  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);


  //Pines de motor 
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);
   pinMode(ENA, OUTPUT);
 

}


void loop()
{
  //  Serial.print(millis());
  //  Serial.print("\t");
  readPosition=as5600.readAngle();
  auxPosition=readPosition-offset;
  Serial.print(readPosition);
  Serial.print("  ");
  Serial.print(auxPosition);
  if (auxPosition<0){
    auxPosition=readPosition+4096-offset;
    }
  
  Serial.print("  ");
  Serial.print(auxPosition);
  Serial.print(" ");
  realPosition=(auxPosition)*360.0/4096;
  if(realPosition>180){
    realPosition=realPosition-360;
    }
  Serial.println(realPosition);
  //Serial.print("\t");
  //Serial.println(as5600.rawAngle());
  //  Serial.println(as5600.rawAngle() * AS5600_RAW_TO_DEGREES);
  pwmPercent=25;
  pwmDuty=int(pwmPercent*255/100);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(IN2, pwmDuty);


}


// -- END OF FILE --
