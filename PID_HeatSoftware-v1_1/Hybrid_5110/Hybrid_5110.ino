// It is assumed that the LCD module is connected to
// the following pins:
//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12
#include <LCD5110_Graph.h>
LCD5110 myGLCD(8,9,10,11,12);
extern uint8_t SmallFont[];

#include <SPI.h>
#include "Adafruit_MAX31855.h"
/* chip select pins used for thermos */
#define CS1 22
#define CS2 24
#define CS3 26
#define CS4 28
#define CS5 40

/* common clock and data pins for thermos */
#define CLK 30
#define DO  32

// initialize the Thermocouple
Adafruit_MAX31855 RT(CLK, CS1, DO);
Adafruit_MAX31855 Th(CLK, CS2, DO);
Adafruit_MAX31855 Tm(CLK, CS3, DO);
Adafruit_MAX31855 Ti(CLK, CS4, DO);
Adafruit_MAX31855 Rm(CLK, CS5, DO);

////////////////////////fan speed////////////////////////////
int NbTopsFan; int Calc;
int hallsensor = 2; typedef struct{
char fantype;
unsigned int fandiv; }fanspec;
fanspec fanspace[3]={{0,1},{1,2},{2,8}}; char fan = 1; 
void rpm ()
{ NbTopsFan++; }
/////////////////////////////////////////////////////////
int minutes = 150; //start min
int targetTh = 94, targetTm = 67, targetTi = 39;
/////////////////////////////////////////////////////////
int seconds = 0; //start seconds
double RT1 = 0, Rm1 = 0, Th1 = 0, Tm1 = 0, Ti1 = 0; 
int nowRT = 0, nowRm = 0, nowTh = 0, nowTm = 0, nowTi = 0; 
int ctTh = 0, ctTm = 0, ctTi = 0; 
int set = 0, u = 0;
int y = 0, x = 0, i = 0;
int alarmpin = 7, alarmcount = 0;
int Threlaypin = 34, Tmrelaypin = 36, Tirelaypin = 38;
int up = 3, down = 4, left = 5, right = 6;

void setup() 
{
   Serial.begin(9600);
   myGLCD.InitLCD();
   myGLCD.setFont(SmallFont);
   myGLCD.clrScr();
   
   pinMode(alarmpin,OUTPUT);
   pinMode(Threlaypin,OUTPUT);  
   pinMode(Tmrelaypin,OUTPUT); 
   pinMode(Tirelaypin,OUTPUT); 
   pinMode(up,INPUT);
   pinMode(down,INPUT);  
   pinMode(left,INPUT); 
   pinMode(right,INPUT);
   digitalWrite(Threlaypin, LOW);
   digitalWrite(Tmrelaypin, LOW);
   digitalWrite(Tirelaypin, LOW);
   
   pinMode(hallsensor, INPUT);
   attachInterrupt(0, rpm, RISING);
}
 
void loop() 
{
      
   myGLCD.print("Advance", CENTER, 20);
   myGLCD.update();
   delay(3000);
    
   while(1)
   {
     RT1 = RT.readCelsius();               //double
     nowRT= RT1;                           //double to int
     Th1 = Th.readCelsius();               //double
     nowTh= Th1;                           //double to int
     Tm1 = Tm.readCelsius();               //double
     nowTm= Tm1;                           //double to int
     Ti1 = Ti.readCelsius();               //double
     nowTi= Ti1;                           //double to int
    // Serial.print(x);
     //Serial.print(y);
     //Serial.print("--");
     //Serial.print(i);
    // Serial.print("\n");

     if(digitalRead(up) == HIGH)
     {
       if(x == 2)
       {
         switch(y)
         {
          case 0:
            targetTh++;
            break;
          case 1:
            targetTm++;
            break;
          case 2:
            targetTi++;
            break;
         }
       }
       else
       {
           i--;
           switch(x)
           {
            case 0:
              if(i<0)
              {y = i = 1;}
              else
              {y = i % 2;}
              break;
            case 1:
              if(i<0)
              {y = i = 3;}
              else
              {y = i % 4;}
              break;
           }
       }
     }
     if(digitalRead(down) == HIGH)
     {
       if(x == 2)
       {
         switch(y)
         {
          case 0:
            targetTh--;
            break;
          case 1:
            targetTm--;
            break;
          case 2:
            targetTi--;
            break;
         }
       }
       else
       {
           i++;
           switch(x)
           {
            case 0:
              y = i % 2;
              break;
            case 1:
              y = i % 4;
              break;
           }
       }
     }
     if(digitalRead(left) == HIGH)
     {
       x--;
       if(x<0) {x = 0;}
     }
     if(digitalRead(right) == HIGH)
     {
       if(x == 1 && y == 3)
       {
          x = 0;
          y = 0; 
       }
       else if(x == 0 && y == 1)
       {
          set = 1;
       }
       else
       {
           x++;
           if(x>2) {x = 2;}
       }
     }
     
//////////////////////////////////////////////////////////////////////////////// 
     if(x == 0)
     {
        myGLCD.clrScr();
        myGLCD.print(">", 0, 32+8*y);
        myGLCD.print("Now Temp.", CENTER, 0);
        myGLCD.drawLine(0, 8, 84, 8);
        myGLCD.drawLine(0, 9, 84, 9);
        
        myGLCD.print("RT:", 0, 12);
        myGLCD.printNumI(nowRT, 18, 12);
        myGLCD.print("~", 30, 12);
        
        myGLCD.print("Th:", 42, 12);
        myGLCD.printNumI(nowTh, 62, 12);
        myGLCD.print("~", 74, 12);
        
        myGLCD.print("Tm:", 0, 20);
        myGLCD.printNumI(nowTm, 18, 20);
        myGLCD.print("~", 30, 20);
        
        myGLCD.print("Ti:", 42, 20);
        myGLCD.printNumI(nowTi, 62, 20);
        myGLCD.print("~", 74, 20);
      
        myGLCD.drawLine(0, 28, 84, 28);
        myGLCD.drawLine(0, 29, 84, 29);
        myGLCD.print("Target Temp.", 8, 32);
        myGLCD.print("Start Heat!!", 8, 40);
        myGLCD.update();
     }
     
    if(x == 1 || x == 2)
    {
      myGLCD.clrScr();
      myGLCD.print(">", 0, 12+8*y);
      myGLCD.print("Tartget Temp.", CENTER, 0);
      myGLCD.drawLine(0, 8, 84, 8);
      myGLCD.drawLine(0, 9, 84, 9);
            
      myGLCD.print("Th:", 8, 12);
      myGLCD.printNumI(targetTh, 26, 12);
      if(targetTh > 99)
      {myGLCD.print("~", 44, 12);}
      else
      {myGLCD.print("~", 38, 12);}
            
      myGLCD.print("Tm:", 8, 20);
      myGLCD.printNumI(targetTm, 26, 20);
      myGLCD.print("~", 38, 20);
            
      myGLCD.print("Ti:", 8, 28);
      myGLCD.printNumI(targetTi, 26, 28);
      myGLCD.print("~", 38, 28);
          
      myGLCD.print("Back", 8, 36);
      myGLCD.update();
    }
//////////////////////////////////////////////////////////////////////////////  

    while (set == 1)
    {
       RT1 = RT.readCelsius();               //double
       nowRT= RT1;                           //double to int
       Rm1 = Rm.readCelsius();               //double
       nowRm= Rm1;                           //double to int
       Th1 = Th.readCelsius();               //double
       nowTh= Th1;                           //double to int
       Tm1 = Tm.readCelsius();               //double
       nowTm= Tm1;                           //double to int
       Ti1 = Ti.readCelsius();               //double
       nowTi= Ti1;                           //double to int
       ////////////////////Fan//////////////////////////
       NbTopsFan = 0;
       sei();
       delay (200);
       cli();
       Calc = ((NbTopsFan * 60)/fanspace[fan].fandiv); 
       ////////////////////Fan//////////////////////////
       u++;
       if(u == 20)
       {
         Serial.print(nowRT); 
         Serial.print("\t"); 
         Serial.print(nowRm); 
         Serial.print("\t"); 
         Serial.print(nowTi); 
         Serial.print("\t"); 
         Serial.print(nowTm); 
         Serial.print("\t"); 
         Serial.print(nowTh); 
         Serial.print("\t"); 
         Serial.print(Calc);   
         Serial.print('\n');   
         u = 0;  
       }
       myGLCD.clrScr();
       //////////////////////RT//////////////////////
       myGLCD.print("RT:", 26, 0);
       myGLCD.printNumI(nowRT, 42, 0);
       myGLCD.print("~", 55, 0);
       //////////////////////RT2//////////////////////
       myGLCD.print("Rm:", 26, 8);
       myGLCD.printNumI(nowRm, 42, 8);
       myGLCD.print("~", 55, 8);
       //////////////////////Th////////////////////// 
       myGLCD.print("Th:", 20, 40);
       myGLCD.printNumI(nowTh, 36, 40);
       if(targetTh > 99)
       {myGLCD.print("~", 55, 40);}
       else
       {myGLCD.print("~", 49, 40);}
       if(ctTh == 1)
       {
            myGLCD.print("OK!", 60, 40);    
       }
       //////////////////////Tm////////////////////// 
       myGLCD.print("Tm:", 20, 29);
       myGLCD.printNumI(nowTm, 36, 29);
       myGLCD.print("~", 49, 29);
       if(ctTm == 1)
       {   
           myGLCD.print("OK!", 60, 29);
       }
       //////////////////////Ti////////////////////// 
       myGLCD.print("Ti:", 20, 18);
       myGLCD.printNumI(nowTi, 36, 18);
       myGLCD.print("~", 49, 18);
       if(ctTi == 1)
       {
          myGLCD.print("OK!", 60, 18); 
       }
       /////////////////////////////////////////
       myGLCD.printNumI(Calc, 60, 29);
       //////////////////////Draw/////////////////////
       myGLCD.drawRect(2, 0, 22, 10);
       myGLCD.drawLine(4, 11, 8, 15);    
       myGLCD.drawLine(20, 11, 16, 15);
       myGLCD.drawLine(8, 15, 16, 15);   //  _
       myGLCD.drawLine(8, 16, 8, 44);    //  |
       myGLCD.drawLine(16, 16, 16, 44);  //  |
       myGLCD.drawLine(8, 44, 12, 48);
       myGLCD.drawLine(16, 44, 12, 48);
       
       myGLCD.drawLine(8, 22, 16, 22);
       myGLCD.drawLine(8, 23, 16, 23);
       
       myGLCD.drawLine(8, 31, 16, 31);
       myGLCD.drawLine(8, 32, 16, 32);
       
       myGLCD.drawLine(8, 42, 16, 42);
       myGLCD.drawLine(8, 43, 16, 43);
       myGLCD.drawLine(8, 44, 16, 44);
       myGLCD.drawLine(10, 45, 14, 45);
       myGLCD.drawLine(11, 46, 13, 46);
       
       myGLCD.update();
    
       heat();   
    }
  }
}

void heat()
{
//////////////////Th//////////////////////////////////   
   if(Th1 < targetTh)                        //heating
   {
     digitalWrite(Threlaypin, HIGH);              //relay start
   }
   else                                         //cooling
   {
     digitalWrite(Threlaypin, LOW);               //relay stop
     ctTh = 1;
   }
   
//////////////////Tm//////////////////////////////////  
   if(Tm1 < targetTm)                        //heating
   {
     digitalWrite(Tmrelaypin, HIGH);              //relay start
   }
   else                                         //cooling
   {
     digitalWrite(Tmrelaypin, LOW);               //relay stop
     ctTm = 1;
   }
   
//////////////////Ti//////////////////////////////////  
   if(Ti1 < targetTi)                        //heating
   {
     digitalWrite(Tirelaypin, HIGH);              //relay start
   }
   else                                         //cooling
   {
     digitalWrite(Tirelaypin, LOW);               //relay stop
     ctTi = 1;
   }
////////////////////////////////////////////////////////   
   if(ctTh == 1 && ctTm ==1 && ctTi ==1)
   {
     if(alarmcount == 0)                       //reach the targettemp, Bi!!
     {
       digitalWrite(alarmpin,HIGH);
       delay(150);
       digitalWrite(alarmpin,LOW);
       delay(150);
       alarmcount = 1;
     }
   }
}
