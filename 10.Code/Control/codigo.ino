const int sensorPin = A2;    // Entrada sensor  
float sensorValue;       // variable que almacena el valor raw (0 a 4095)
//salida motor 2
int IN1 = 6;    
int IN2 = 8;    
int ENA=9;
//
float PWM=0;
float ANG;
float aux;

float kp = 1.78E-3; 
float kd = 1/699.85;
float ki=4.5466E-4;
float CmdPID = 0.0;
float directCmd = 90;
//Control system variables
float E = 0;
float Ep = 0;
float Up = 0;
float Ud = 0;
float Ui = 0;
float Ref = 43; //angulo de referencia


void setup()
{
 pinMode (IN1, OUTPUT);
 pinMode(IN2,OUTPUT);
 pinMode(ENA,OUTPUT);
 Serial.begin(112500);
}



void loop()
{
  
  PWM=90; //angulo 41.84-41.49
  //Sentido del motor
  digitalWrite (ENA,HIGH);  //OUT2 GND
  digitalWrite (IN2, LOW);

  // Giro del motor

  analogWrite(IN1,PWM);
   sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);
  ANG =sensorValue*360/1024 -266.13; // El sensor es de 12 bits, por lo que divide los 360 grados en 4096 niveles
  Serial.println(ANG);
  delay(500);
  

}

  
