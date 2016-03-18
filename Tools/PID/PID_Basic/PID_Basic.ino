#include <math.h>
#include <PID_v1.h>
#define Data_size 5
#define SampleTime 50

// Add some consts of the signal filter
int index=0;
double Temp_array[Data_size];

//Define Variables we'll be connecting to
double PIDtargetTh = 20 , ThVolt, Output, T, R;
const int Thtransistorpin = A14;                 // Analog output pin that the transistor is attached to
const int Thermistorpin = A7;
int sensorValue = 0;

//Specify the links and initial tuning parameters
double Kp=256, Ki=0, Kd=0;
PID myPID(&T, &ThVolt, &PIDtargetTh, Kp, Ki, Kd, REVERSE);

void setup()
{
  Serial.begin(9600);
  myPID.SetOutputLimits(0, 255);
  myPID.SetSampleTime(SampleTime);
  myPID.SetMode(AUTOMATIC);
}

void loop()
{
  thermistor();
  myPID.Compute();
  analogWrite(Thtransistorpin, ThVolt);
  Serial.print("Temperature\t");
  Serial.print(T);
  Serial.print("\tVolt%\t");
  Serial.println(ThVolt);
}

void thermistor(){
  T=0;
  sensorValue = analogRead(Thermistorpin);            
  R = (4.7*sensorValue)/(1023-sensorValue);
  Temp_array[index] = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
  
  if(index<Data_size){
    index++;
  }
  else{
    index=0;
  }
  for(int i=0;i<Data_size;i++){
    T+=Temp_array[i];
  }
  T=T/Data_size;
}

