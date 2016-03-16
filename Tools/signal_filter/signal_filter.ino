#include <Average.h>
#define Data_size 5

int Time,index=0,counter=0;
double Temp_array[Data_size];
double T,Temp=0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  T=0;
  if(index<Data_size){
    Temp_array[index]=signalvalue();
    index++;
  }
  else{
    index=0;
  }
  
  for(int i=0;i<Data_size;i++){
    T+=Temp_array[i];
  }
  T=T/Data_size;
  if(millis()%100 ==0){
    Time = millis();
    Serial.print("Temp: ");
    Serial.println(T); 
  }
}

double signalvalue(){
  double sig=0,k=0;
  int i=25;
    sig=0;
    k=0;
    for(int j=0;j<=2;j++){
      k=random(10);
      k/=10;
      sig+=k;
      }
    sig+=i;
  return sig;
}

