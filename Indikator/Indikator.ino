/**
Indikator v 1.0
**/

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int buttonPin = 7;
int biasPin = 6;

int v [] = {0, 1, 1, 0, 0, 1, 1, 0};


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(buttonPin, INPUT);
  pinMode(biasPin, INPUT);
  delay(1);
  
  startupRoutine();
}

void startupRoutine(){
  for (int i = 0; i < 8; i++){
    writeSR(i);
    delay(10);
  }
  
  delay(1000);
  writeSR(0);
}

void loop() {
  int value = 0 ;
  
 while (digitalRead(buttonPin) == LOW){
   delay(1);
 }
 
 while (digitalRead(buttonPin) == HIGH){
   writeSR(0);
   value = (random(1000,6000) + bias()) / 1000;
   delay(1);
 }
 
 writeSR(value);
 delay(1);
}

int bias(){
  return digitalRead(biasPin) - 512;
}


void writeSR(int value){
  digitalWrite(latchPin, LOW);
  delay(1);
  
  for (int i = 0; i < value; i++){
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, 1);
    digitalWrite(clockPin, HIGH);
    delay(1);
  }
  digitalWrite(latchPin, HIGH);
}

