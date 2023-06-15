//Encoder
int sensorPin = A2; //Pin 14
float sensorValue = 0;

float startAngle = 0; //starting angle
float degAngle; //Angulo actual

//Motor
int ENA = 6; //Puente H enable
int IN1 = 8; //Puente H IN3
int IN2 = 9; //Puente H IN4 pwm

//Control
float Ref = 43; // System Reference - Angle[º]
unsigned int pwmDuty = 0;
float directCmd = 30; //PWM de operacion

float Cmd = 0.0; // Control Input
float CmdPID = 0.0; // Control Output
float CmdPIDp = 0;


//Ganancias de control
float k_p = 1.78E-3;
float k_d = 1/699.85;
float k_i=4.5466E-4;
//Orden del filtro
float N = 100; 

float T = 0.0077;
float b = 4.8865;
//Errores
float E = 0; 
float Ep = 0;
float Ep2 = 0;
//Accionnes de control
float U = 0;
float Up = 0; 
float Up2 = 0;
float U_i=0;
float U_ip=0;
float U_d=0;
float U_dp=0;
float U_p=0;
float Upid=0;
float Upidp=0;
//Constantes del compensador
const float c=175.21; //constante del comp
const float z=2.8865; //cero
const float p=9.91; //polo
const int kc=30; //ganancia
unsigned long currentMillis;

long Ts = 1; // Sample time in ms

long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  
  sensorValue = analogRead(sensorPin);
  degAngle = sensorValue*360/1023;
  startAngle = degAngle; //Angulo de tara

  pinMode(sensorPin, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
}

void loop() {
  //PuenteH();
  //readAngle();
  currentMillis = millis(); // Update current time from the beginning
  controlPID();
}

void controlPID(void){

  if (currentMillis - previousMillis >= Ts) {
    
    previousMillis = currentMillis;
    sensorValue = analogRead(sensorPin);
    degAngle = startAngle-sensorValue*360/1023; //Conversion de Analogo a Angulo

    float CmdLim = min(max(CmdPID, -20), 20); // Saturated Control Output. Asumiendo el PWM inicial en 40
    float Cmd = CmdLim + directCmd;    
    pwmDuty = int((Cmd/100)*255); //Conversion de cmd a pwm y activacion del motor
    digitalWrite(ENA, HIGH);
    digitalWrite(IN1, LOW);
    analogWrite(IN2, pwmDuty);
    Serial.print("Funciona");
    if (currentMillis > 10000) {
      E = Ref - degAngle;
      U_i=U_ip+(k_i*T*Ep);
      U_d=U_dp*((N*T)-1)+k_d*N*E-k_d*N*Ep;
      U_p=k_p*E;
      Upid=U_p+U_i+U_d;
      U=(1/((2/T)+p))*Upid*((2*c*kc/T)+kc*c*z)+Upidp*((-2*kc*c/T)+c*z*kc)+Up*((2/T)-p);
      x|  x 
      //U = ((2*k_i + 4*k_p + 4*N*k_d + N*k_i + 2*N*k_p)*E + (2*N*k_i - 8*N*k_d - 8*k_p)*Ep + (4*k_p - 2*k_i + 4*N*k_d + N*k_i - 2*N*k_p)*Ep2 + 8*Up - (4 - 2*N)*Up2) / (2*N+4);
      CmdPID = ((2*T*b + 1)*U + (1 - 2*T*b)*Up - (1 - 2*T)*CmdPIDp) / (2*T+1);
      //Ep2 = Ep;
      Ep = E;
      //Up2 = Up;
      Up = U;
      Upidp=Upid;
      U_ip=U_i;
      U_dp=U_d;
      CmdPID = CmdPIDp;
      Serial.print("Error:");
      Serial.print(E);
      Serial.print(",");
      Serial.print("U:");
      Serial.print(CmdPID);
      Serial.print(",");
      Serial.print("Angulo:");
      Serial.println(degAngle);
    }  
  }
}

void readAngle(){
  sensorValue = analogRead(sensorPin);
  degAngle = startAngle-sensorValue*360/1023; //Conversion de Analogo a Angulo
}

void PuenteH(){
  //Conversion de cmd a pwm y activacion del motor
  float CmdLim = min(max(CmdPID, -40), 60); // Saturated Control Output. Asumiendo el PWM inicial en 40
  float Cmd = CmdLim + directCmd;    
  pwmDuty = int((Cmd/100)*255);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);
  analogWrite(IN2, pwmDuty);
}
