int dataPin = 2;
int clockPin = 3;
int latchPin = 4;
int speakerPin = 5;
int errorLedPin = 6;
int casinoTogglePin = 7;
int slowTogglePin = 8;
int multipleTogglePin = 9;
int rollButton = 10;
int cheatTogglePin = 11;
int cheatPotPin = A0;
int casinoPotPin = A1;
int slowPotPin = A2;
int multiplePotPin = A3;


void setup() {

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(errorLedPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(casinoTogglePin, INPUT);
  pinMode(slowTogglePin, INPUT);
  pinMode(multipleTogglePin, INPUT);
  pinMode(rollButton, INPUT);
  pinMode(cheatTogglePin, INPUT);
  digitalWrite(casinoTogglePin, HIGH);
  digitalWrite(slowTogglePin, HIGH);
  digitalWrite(multipleTogglePin, HIGH);
  digitalWrite(rollButton, HIGH);

  writeSR(0,false);
  delay(500);
  for(int i = 1; i <= 10; i++) {
    soundSpeaker(i,20);
    writeSR(i,false);
  }

}




void loop() {

  digitalWrite(errorLedPin, LOW);

  long counter = 0;
  while(digitalRead(rollButton) == HIGH) {
    counter++;
    delay(1);
    if(counter == 60000) {
      writeSR(0,false);
    }
  }
  counter == 0;

  while(digitalRead(rollButton) == LOW) {

    int casino = digitalRead(casinoTogglePin);
    int slow = digitalRead(slowTogglePin);
    int multiple = digitalRead(multipleTogglePin);
    int sum = casino + slow + multiple;


    if(sum == 0 || sum == 1){
      digitalWrite(errorLedPin, HIGH);
      soundSpeaker(1,5);
      delay(1);
    }
    else {
      if(casino == 0) {
        casinoRoll();
      }
      else if(slow == 0) {
        slowRoll();
      }
      else if(multiple == 0){
        multipleRoll();
      }
      else {
        normalroll();
      }
    }
  }

}

void casinoRoll(){
  while(digitalRead(rollButton) == LOW) {
    writeSR(rollDice(), false);
    soundSpeaker(10,1);
    int d = (210 - (readPot(casinoPotPin))/5);
    delay(d);
  }
  writeSR(rollDice(), true);
}

void slowRoll(){
  while(digitalRead(rollButton) == LOW){
    delay(1);
  }

  int t = (readPot(slowPotPin)/100)+4;
  int v[6];

  for (int i = 0; i < t*2; i++){
    for (int j = 0; j < 6; j++){
      v[j] = random(0,2);
    }
    writeSR(v);
    soundSpeaker(10,1);
    delay(500);
  }

  for (int i = 0; i < 3; i++){
    writeSR(6, false);
    soundSpeaker(5,20);
    delay(167);
    writeSR(0, false);
    delay(167);
  }
  writeSR(rollDice(), true);
}


void multipleRoll(){
  int amount = 0;
  int value = 0;

  while(digitalRead(rollButton) == LOW){
    delay(1);
  }
  acknowledge();

  while(digitalRead(rollButton) == HIGH){
    amount = (int)((readPot(multiplePotPin) *62.0/1023.0) + 1.0);
    showResults(amount);
    delay(1);

  }


  while(digitalRead(rollButton) == LOW){
    delay(1);
  }
  acknowledge();


  while(digitalRead(rollButton) == HIGH){
    value = (int)((readPot(multiplePotPin) * 5.0 / 1023.0) +1);
    writeSR(value, false);
    delay(1);

  }


  while(digitalRead(rollButton) == LOW){
    delay(1);
  }

  generateResults(amount, value);

}

void generateResults(int amount, int value){
  int roll = 0;
  int hit = 0;

  for (int i = 0; i < amount; i++){
    roll = rollDice();
    if (roll >= value){
      hit++;

    }

  }

  showResults(hit);
  soundSpeaker(8,5);
}

void showResults(int v){
  int results[6];

  for (int i = 5; i>=0; i--){
    int rest = v%2;
    results[i] = rest;
    v = v/2;
  }

  writeSR(results);
}

void acknowledge(){
  writeSR(6, false);
  soundSpeaker(5,5);
  delay(200);
  writeSR(0, false);
  delay(200);
  writeSR(6, false);
  soundSpeaker(5,5);
  delay(200);
  writeSR(0, false);
  delay(1);
}




void normalroll(){
  while(digitalRead(rollButton) == LOW){
    delay(1);
  }

  writeSR(rollDice(), true);
  soundSpeaker(10,1);
  delay(1);
}



int rollDice(){
  int r = random(1000,6000);

  if(digitalRead(cheatTogglePin) == LOW){
    r += bias();
  }

  r = (r+500)/1000;

  if (r > 6){
    r = 6;
  }
  if (r < 1){
    r = 1;
  }


  return r;
}

int bias(){
  int b = readPot(cheatPotPin) - 512;
  return b * 4;
}



int readPot(int p){
  return 1023 - analogRead(p);
}


void writeSR(int v[]){
  digitalWrite(latchPin, LOW);
  delay(1);

  for (int i = 0; i < 2; i++){
    helpSR(0);
  }

  for (int i = 5; i >= 0; i--){
    helpSR(v[i]);
  }

  digitalWrite(latchPin, HIGH);
}


void writeSR(int value, boolean sound){
  digitalWrite(latchPin, LOW);
  delay(1);

  for (int i = 0; i < 8 - value; i++){
    helpSR(0);
  }

  for (int i = 0; i < value; i++){
    helpSR(1);
  }

  digitalWrite(latchPin, HIGH);
  if(sound) {
    if(value == 1) {
      for(int i = 5; i >= 1 ; i--) { 
        soundSpeaker(i,20);
      }
    }
    if(value == 6) {
      for(int i = 1; i <= 10; i++) {
        soundSpeaker(i,10);
      }
    }
  }

}


void helpSR(int v){
  digitalWrite(clockPin, LOW);
  digitalWrite(dataPin, v);
  digitalWrite(clockPin, HIGH);
  delay(1);
}


void soundSpeaker(int level, long time) {
  double e = -2.0/9.0*(double)level + 4.0;
  long d = pow(10,e);
  long elapsedTime = 0;

  while(elapsedTime < time*1000) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(d);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(d);
    elapsedTime = elapsedTime + d;
  }
}

