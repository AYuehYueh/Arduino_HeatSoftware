#include <PID_v1.h>
#define Data_size 10
#define SampleTime 100
#define DispTime 100
#define Transistor A14
#define Thermistor A7

// Add some consts of the signal filter
int index=0;
double Temp_array[Data_size];

//Define Variables we'll be connecting to
double PIDtargetTh = 94 , ThVolt, Output, T, R;
const int ThTransistorPin = Transistor;                                                  // Analog output pin that the transistor is attached to
const int ThermistorPin = Thermistor;
int sensorValue = 0, LastTime = 0 ;

//Specify the links and initial tuning parameters
double Kp=256, Ki=0, Kd=0;
PID myPID(&T, &ThVolt, &PIDtargetTh, Kp, Ki, Kd, DIRECT);

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
  analogWrite(ThTransistorPin, ThVolt);
  if(millis()-LastTime >= DispTime){
    LastTime=millis();
    Serial.print("Temperature\t");
    Serial.print(T);
    Serial.print("\tVolt%\t");
    Serial.println(ThVolt); 
  }
}

void thermistor(){
  T=0;
  sensorValue = analogRead(ThermistorPin);            
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

