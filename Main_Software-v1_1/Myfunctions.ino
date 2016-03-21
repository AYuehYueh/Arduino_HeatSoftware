int page_change(int page_index,int directions){                                            // directions (0 is decrease,1 is increase)
  if(directions == 0){
    if(page_index+page_size > 0){
      page_index--; 
    }
  }
  else{
    if(page_index-page_size < 0){
      page_index++;
    }
  }
  return page_index;
}

int option_change(int option_size,int option_index,int directions){                         // option_size,option_index,directions(0 is decrease,1 is increase)
  if(directions == 0){
    if(option_index <= option_size && option_index > 0){
      option_index--;
    }
    else{
      option_index=0;
    }
  }
  else if (directions == 1){
    if(option_index < option_size){
      option_index++;
    }
    else{
      option_index=0;
    }
  }
  return option_index;
}

void number_change(int page_index,int option_index,int directions){
  if (directions==0){
    if(page_index == -2){
        switch(option_index){
          case 0:
            Kp++;
          break;
          case 1:
            Ki=Ki+0.01;
          break;
          case 2:
            D=D+0.01;
          break;
        }      
      }
    else if(page_index == 2){
      switch(option_index){
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
  }
  else{
    if(page_index == -2){
        switch(option_index){
          case 0:
            Kp--;
          break;
          case 1:
            Ki=Ki-0.01;
          break;
          case 2:
            D=D-0.01;
          break;
        }      
      }
    else if(page_index == 2){
      switch(option_index){
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
  }
}

void LCDdisplay(int xIndex,int yIndex,bool Toheat){
  Th1=T;                                                                              // needs to be adjusted
  
  switch(Toheat){
    case false:
      if(xIndex == 0){
        myGLCD.clrScr();
        myGLCD.print(">", 0, 32+8*yIndex);
        myGLCD.print("Now Temp.", CENTER, 0);
        myGLCD.drawLine(0, 8, 84, 8);
        myGLCD.drawLine(0, 9, 84, 9);
        
        myGLCD.print("RT:", 0, 12);
        myGLCD.printNumI(RT1, 18, 12);
        myGLCD.print("~", 30, 12);
        
        myGLCD.print("Th:", 42, 12);
        myGLCD.printNumI(Th1, 62, 12);
        myGLCD.print("~", 74, 12);
        
        myGLCD.print("Tm:", 0, 20);
        myGLCD.printNumI(Tm1, 18, 20);
        myGLCD.print("~", 30, 20);
        
        myGLCD.print("Ti:", 42, 20);
        myGLCD.printNumI(Ti1, 62, 20);
        myGLCD.print("~", 74, 20);
        
        myGLCD.drawLine(0, 28, 84, 28);
        myGLCD.drawLine(0, 29, 84, 29);
        myGLCD.print("Target Temp.", 8, 32);
        myGLCD.print("Start Heat!!", 8, 40);
        myGLCD.update();
      }
      if(xIndex > 0){
        myGLCD.clrScr();
        myGLCD.print(">", 0, 12+8*yIndex);
        myGLCD.print("Tartget Temp.", CENTER, 0);
        myGLCD.drawLine(0, 8, 84, 8);
        myGLCD.drawLine(0, 9, 84, 9);
              
        myGLCD.print("Th:", 8, 12);
        myGLCD.printNumI(targetTh, 26, 12);
        if(targetTh > 99){
          myGLCD.print("~", 44, 12);
        }
        else{
          myGLCD.print("~", 38, 12);
        }    
        myGLCD.print("Tm:", 8, 20);
        myGLCD.printNumI(targetTm, 26, 20);
        myGLCD.print("~", 38, 20);
              
        myGLCD.print("Ti:", 8, 28);
        myGLCD.printNumI(targetTi, 26, 28);
        myGLCD.print("~", 38, 28);
        myGLCD.update();
      }
      if(xIndex < 0){
        myGLCD.clrScr();
        myGLCD.print(">", 0, 12+8*yIndex);
        myGLCD.drawLine(0, 8, 84, 8);
        myGLCD.drawLine(0, 9, 84, 9);
              
        myGLCD.print("Kp:", 8, 12);
        myGLCD.printNumI(Kp, 26, 12);          
        myGLCD.print("Ti:", 8, 20);
        myGLCD.printNumI(Ki, 26, 20);        
        myGLCD.print("Td:", 8, 28);
        myGLCD.printNumI(D, 26, 28);
        myGLCD.update();
      }
    break; 
    
    case true:
      myGLCD.clrScr();
      myGLCD.print("RT:", 26, 0);
      myGLCD.printNumI(RT1, 42, 0);
      myGLCD.print("~", 55, 0);
      myGLCD.print("Rm:", 26, 8);
      myGLCD.printNumI(Rm1, 42, 8);
      myGLCD.print("~", 55, 8);
      myGLCD.print("Th:", 20, 40);
      myGLCD.printNumF(T,1, 36, 40);
      if(targetTh > 99){
        myGLCD.print("~", 68, 40);}
      else{
        myGLCD.print("~", 63, 40);}
      if(Th1 == targetTh){
        myGLCD.print("OK!", 60, 40);    
      }
        myGLCD.print("Tm:", 20, 29);            
        myGLCD.printNumI(Tm1, 36, 29);
        myGLCD.print("~", 49, 29);
      if(Tm1 == targetTm){   
        myGLCD.print("OK!", 60, 29);
      }
      myGLCD.print("Ti:", 20, 18);
      myGLCD.printNumI(Ti1, 36, 18);
      myGLCD.print("~", 49, 18);
      if(Ti1 == targetTi){
        myGLCD.print("OK!", 60, 18); 
      }
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
  }
}

void thermistor(){
  sensorValue = analogRead(TherMistorPin);            
  R = (4.7*sensorValue)/(1023-sensorValue);
  T = 162.1-33.76*log(R)+0.18898*log(R)*log(R)*log(R);
}

void heat(){
//  if(tuning){                                                                     // let's test!!
//    byte val = (aTune.Runtime());
//    if (val!=0){
//      tuning = false;
//    }
//    if(!tuning){                                                                  //we're done, set the tuning parameters
//      Kp = aTune.GetKp();
//      Ki = aTune.GetKi();
//      D = aTune.GetKd();
//      myPID.SetTunings(Kp,Ki,D);
//    }
//  }
//  else myPID.Compute();
  myPID.Compute();
  analogWrite(ThTransistorPin, ThVolt);

}
