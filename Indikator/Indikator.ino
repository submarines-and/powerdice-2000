/**
Indikator v 1.0
**/

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int v [] = {0, 1, 1, 0, 0, 1, 1, 0};


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  digitalWrite(latchPin, LOW);
  delay(1);
  
  for (int i = 0; i < 8; i++){
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, v[i]);
    digitalWrite(clockPin, HIGH);
    delay(1);
  }
  digitalWrite(latchPin, HIGH);

}

