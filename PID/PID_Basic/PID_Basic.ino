/********************************************************
 * PID Basic Example
 * Reading analog input 0 to control analog PWM output 3
 ********************************************************/
#include <math.h>
#include <PID_v1.h>


//Define Variables we'll be connecting to
double PIDtargetTh, ThVolt, Output;
const int Thtransistorpin = 7;                 // Analog output pin that the transistor is attached to
const int Thermistorpin = A7;

double T,R;  
int sensorValue = 0;

//Specify the links and initial tuning parameters
double Kp=4.8, Ki=35, Kd=9;
PID myPID(&T, &ThVolt, &PIDtargetTh, Kp, Ki, Kd, DIRECT);

void setup()
{
  //initialize the variables we're linked to
  Serial.begin(9600);
  thermistor();
  PIDtargetTh = 94;
  Serial.print(T);

  //turn the PID on
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  thermistor();
  myPID.Compute();
  analogWrite(Thtransistorpin, 255);
  Serial.print("Temperature\t");
  Serial.print(T);
  Serial.print("\tVolt%\t");
  Serial.println(ThVolt);
  delay(50);
}


void thermistor(){
  sensorValue = analogRead(Thermistorpin);            
  R = (4.7*sensorValue)/(1023-sensorValue);
  T = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
}

