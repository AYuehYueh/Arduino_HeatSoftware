#include <math.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
#include <LCD5110_Graph.h>
#include <Adafruit_MAX31855.h>
#define CS1 22
#define CS2 24
#define CS3 26
#define CS4 28
#define CS5 40
#define CLK 30                                                                            // common clock and data pins for thermos
#define DO  32                                                                            // common clock and data pins for thermos

LCD5110 myGLCD(8,9,10,11,12);
extern uint8_t SmallFont[];

Adafruit_MAX31855 RT(CLK, CS1, DO);                                                       // initialize the Thermocouple
Adafruit_MAX31855 Rm(CLK, CS5, DO); 
Adafruit_MAX31855 Th(CLK, CS2, DO);
Adafruit_MAX31855 Tm(CLK, CS3, DO);
Adafruit_MAX31855 Ti(CLK, CS4, DO);

const int up = 3, down = 4, left = 5, right = 6;
const int opt_size[]={2,1,2};                                                               // sizes of the options which are in the pages
const int ThTransistorPin = 7, TmTransistorPin = 36, TiTransistorPin = 38;                 // Analog output pin that the transistor is attached to
const int TherMistorPin = A7;

double P = 4,I = 0.01, D = 0.005 ;                                                                 // Determines how aggressively the PID reacts to ( order: Th,Ti,Tm )
double ThVolt;
double PIDtargetTh;
boolean Toheat = false,tuning = false;
int RT1 , Rm1 , Th1 , Tm1 , Ti1 = 0; 
int targetTh = 94,targetTi = 67,targetTm = 39;
int page_size=4;    
int ctlpage[] = { 0, 0 };                                                      
int sensorValue = 0;
double R;
double T;                                                         // temperature of thermistor
int counter=0;


PID myPID(&T, &ThVolt, &PIDtargetTh, P, I, D, DIRECT);
//PID_ATune aTune(&T, &ThVolt);

void setup() {
  Serial.begin(9600);
  myPID.SetSampleTime(100);
  myPID.SetMode(MANUAL);
//  myPID.SetOutputLimits(0,1023);
//  aTune.SetControlType(1);                                                                // Sets whether PI or PID tuning parameters will be returned when GetKp et al are called.
//  aTune.SetOutputStep(4500);                                                              // Sets How much the output will be stepped above and below the starting point (still need to adjust)
//  aTune.SetNoiseBand(0.5);                                                                // Sets what level of input noise will be ignored before stepping the output  (still need to adjust)
//  aTune.SetLookbackSec(30);                                                               // Sets How far back the tuner will look when determining if this point is, in fact, a min or a max (still need to adjust)
  myGLCD.InitLCD();                                                                         // seconds: integer. think about how far apart the peaks are. 1/4-1/2 of this distance is a good value
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myGLCD.print("Advance", CENTER, 20);
  myGLCD.update();
  pinMode(ThTransistorPin, OUTPUT); 
  pinMode(A11, OUTPUT); 
  pinMode(TherMistorPin, INPUT); 
  pinMode(up,INPUT);
  pinMode(down,INPUT);  
  pinMode(left,INPUT); 
  pinMode(right,INPUT);
  page_size=page_size/2;
  delay(2000); 
}

void loop() {
//  RT1 = RT.readCelsius();              
//  Th1 = Th.readCelsius();              
//  Tm1 = Tm.readCelsius();               
//  Ti1 = Ti.readCelsius();
  thermistor(); 
  PIDtargetTh = targetTh;           
  switch(Toheat){
    case false:
      if(digitalRead(up) == HIGH || digitalRead(down) == HIGH){
        if(ctlpage[0] == -2 || ctlpage[0] == 2){
          number_change(ctlpage[0],ctlpage[1],digitalRead(down));
        }
        else{
          ctlpage[1]=option_change(opt_size[ctlpage[0]+1],ctlpage[1],digitalRead(down));
        }
      }
      if(digitalRead(right)== HIGH || digitalRead(left)== HIGH){
        if(digitalRead(right) == HIGH && ctlpage[0] == 0 && ctlpage[1] == 1){
          Toheat=true;
          myPID.SetTunings(P,I,D);
          myPID.SetMode(AUTOMATIC);                                                                 // turn the PID on
        }
        else{    
          ctlpage[0]=page_change(ctlpage[0],digitalRead(right));
          if(abs(ctlpage[0])<=1){
            ctlpage[1]=0;
          }
        }
      }
      LCDdisplay(ctlpage[0],ctlpage[1],Toheat);
    break;
    
    case true:
      LCDdisplay(ctlpage[0],ctlpage[1],Toheat);
      heat();
      break;
  }
  counter++;
  if (counter==10){
    Serial.print(T);
    Serial.print("\t");
    Serial.println(ThVolt);
    counter=0;
  }
  delay(50);
}
