#include <math.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
                                                         // sizes of the options which are in the pages
const int Thtransistorpin = 7;                 // Analog output pin that the transistor is attached to
const int Thermistorpin = A7;

double P = 2,I = 0, D = 0 ;                                                                 // Determines how aggressively the PID reacts to ( order: Th,Ti,Tm )
double ThVolt;
double PIDtargetTh;
boolean Toheat = false,tuning = true;
int targetTh = 94;
int sensorValue = 0;
double R;
double T;                                                         // temperature of thermistor


PID myPID(&T, &ThVolt, &PIDtargetTh, P, I, D, DIRECT);
PID_ATune aTune(&T, &ThVolt);

void setup() {
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);                                                               // turn the PID on
  myPID.SetSampleTime(100);
  aTune.SetControlType(1);                                                                // Sets whether PI or PID tuning parameters will be returned when GetKp et al are called.
  aTune.SetOutputStep(1);                                                              // Sets How much the output will be stepped above and below the starting point (still need to adjust)
  aTune.SetNoiseBand(0.8);                                                                // Sets what level of input noise will be ignored before stepping the output  (still need to adjust)
  aTune.SetLookbackSec(5); 
  PIDtargetTh = targetTh;  
  Serial.println("Start!!");  
}

void loop() {
  thermistor();
  heat(); 
  Serial.print("Kp:\t");
  Serial.println(P);
  delay(10);
}

void heat(){
  if(!tuning){                                                                     // let's test!!
    byte val = (aTune.Runtime());
//    if (val!=0){
//      tuning = false;
//    }
    if(!tuning){                                                                  //we're done, set the tuning parameters
      P = aTune.GetKp();
      I = aTune.GetKi();
      D = aTune.GetKd();
      myPID.SetTunings(P,I,D);
    }
  }
  else myPID.Compute();
  analogWrite(Thtransistorpin, ThVolt);
  Serial.print("Kp:\t");
  Serial.print(P);
  Serial.print("\tKi:\t");
  Serial.print(I);
  Serial.print("\tKd:\t");
  Serial.print(D);
  Serial.print("\tTemperature\t");
  Serial.print(T);
  Serial.print("\tVolt\t");
  Serial.println(ThVolt);

  }

void thermistor(){
  sensorValue = analogRead(Thermistorpin);            
  R = (4.7*sensorValue)/(1023-sensorValue);
  T = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
}
