int Time,counter=0;
double T,Temp=0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  if(counter<=9){
    Temp+=signalvalue();
    counter++;
  }
  else{
    Temp/=10;
    T=Temp;
    Temp=0;
    counter=0;
  }
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

