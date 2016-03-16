#include <Stepper.h>   //  E232171 and L293D

#define STEPS 20       // A circle has 20steps.
#define cartridge 32

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 9, 10, 11);
int location = 0;
int steps = 0;
int command=0;
void setup()
{
  // set the speed of the motor 
  stepper.setSpeed(300);
  Serial.begin(9600);
  pinMode(cartridge, OUTPUT);
}

void loop()
{
  if (Serial.available() > 0){
    command=Serial.read();
    command-=48;
    if (command==0){
      steps=(command*120)-location;
      location=command*120;
      }
    else if (command==1){
      steps=(command*120)-location;
      location=command*120;
      digitalWrite(cartridge,HIGH);
    }
    else if (command==2){
      steps=(command*120)-location;
      location=command*120;
      digitalWrite(cartridge,HIGH);
    }
    else if (command==3){
      steps=(command*120)-location;
      location=command*120;
      digitalWrite(cartridge,HIGH);
    }
    delay(50);
    Serial.print("Tip:\t");
    Serial.print(command);
    Serial.print("\tLocation:\t");
    Serial.println(location);
    stepper.step(steps);
  } 
  digitalWrite(cartridge,LOW);
  delay(450);

}
