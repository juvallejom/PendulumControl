//Control PID de balancin

#include <PID_v1.h>

const int Pot = A2;       //pin analogico lectura del potenciometro
//Define Variables
double Setpoint, Input, Output;
//Iniciar parametros
double Kp=1.78E-3, Ki=1/699.85, Kd=4.5466E-4;   //por prueba y error
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
unsigned long previousMillis = 0;   //Actualiza gráfica
const long interval = 100;         //Gráfica cada 1/2''
int IN1 = 6;    // Input3 conectada al pin 5
int IN2 = 8;    // Input4 conectada al pin 4
int ENA = 9;    // ENB conectada al pin 3 de Arduino
float value=0;

void setup(){
  Serial.begin(115200);
  //pinMode (ENA, OUTPUT);
  //pinMode (IN1, OUTPUT); //PIN SALIDA PWM
  //pinMode (IN2, OUTPUT);
  digitalWrite (IN2, LOW); 
  digitalWrite(ENA,HIGH);
  Setpoint = 90;           //Que queremos mantener
  myPID.SetSampleTime(1);
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255,255);
}

void loop(){
  unsigned long currentMillis = millis();
  value = analogRead(Pot);    //Valor del sensor
  Output=30;
  Input =(value*360/1024 -266.13)+42;
 
  myPID.Compute();  
  analogWrite(IN1, Output);   //Salida al motor
  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;
   Serial.print(",");   
   Serial.print("SALIDA PWM:");
   Serial.print(Output);   //PWM
   Serial.print(",");   
   //Serial.print(Setpoint);      //referencia en PWM
   Serial.print(",");  
   Serial.print("ENTRADA PWM:");
   Serial.println(Input-42);  // entrada (PWM)
   Serial.print(",");  
   Serial.print("ANGULO:");
   Serial.print(Input);
  }
               //Calculo por el PID 
}
