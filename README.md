# Bop-It
int ledPin = 13;
long counter = 0;
long level = 0;
long tijd = 5000;
int picker;
bool inHetSpel = true;
 
int buttonPush = 6;
int buttonPull = 2;
int buttonTwist = 3;
int ledRood = 4;
int ledGroen = 5;
int lastTime = 0;
int reactieTijd = 2000;
int startLamp;
 
void setup() {
Serial.begin(115200);
pinMode(ledPin, OUTPUT);
pinMode(buttonPush, INPUT_PULLUP);
pinMode(buttonPull, INPUT_PULLUP);
pinMode(buttonTwist, INPUT_PULLUP);
pinMode(ledGroen, OUTPUT);
pinMode(ledRood, OUTPUT);
}
 
 
 
void loop() {
    
  while(inHetSpel == true) {
  //while(dimmed == true);  
  picker = random(1,4);
  switch(picker){
    case 1:
      twistIt();
      break;
    case 2:
      bopIt();
      break;
    case 3:
      pullIt();
      break;
    default:
      break;  
    }
 
 
 if (counter >= 10) {
    counter = 0;
    level = level +1;
    tijd = tijd -2000;
    Serial.println(level);
    //lose = true;
    }
  }
}

 
//if (level == 4) {
 // inHetSpel = false;
  //Serial.println("je hebt gewonnen!!")
 //}
 
void bopIt() {
  Serial.println("bopIt");
  unsigned long startTijd = millis();
  bool correct = false;

  while (millis() - startTijd < reactieTijd) {
    if (digitalRead(buttonPush) == LOW) {
      correct = true;
      break;
    }
  }

  if (correct) {
    digitalWrite(ledGroen, HIGH);
    Serial.println("Goed!");
    counter++;
    delay(1000);
    digitalWrite(ledGroen, LOW);
  } else {
    digitalWrite(ledRood, HIGH);
    Serial.println("Fout!");
    delay(1000);
    digitalWrite(ledRood, LOW);
  }

  delay(tijd);
}

void pullIt() {
  Serial.println("pullIt");
  unsigned long startTijd = millis();
  bool correct = false;

  while (millis() - startTijd < reactieTijd) {
    if (digitalRead(buttonPull) == LOW) {
      correct = true;
      break;
    }
  }

  if (correct) {
    digitalWrite(ledGroen, HIGH);
    Serial.println("Goed!");
    counter++;
    delay(1000);
    digitalWrite(ledGroen, LOW);
  } else {
    digitalWrite(ledRood, HIGH);
    Serial.println("Fout!");
    delay(1000);
    digitalWrite(ledRood, LOW);
  }

  delay(tijd);
}

void twistIt() {
  Serial.println("twistIt");
  unsigned long startTijd = millis();
  bool correct = false;

  while (millis() - startTijd < reactieTijd) {
    if (digitalRead(buttonTwist) == LOW) {
      correct = true;
      break;
    }
  }

  if (correct) {
    digitalWrite(ledGroen, HIGH);
    Serial.println("Goed!");
    counter++;
    delay(1000);
    digitalWrite(ledGroen, LOW);
  } else {
    digitalWrite(ledRood, HIGH);
    Serial.println("Fout!");
    delay(1000);
    digitalWrite(ledRood, LOW);
  }

  delay(tijd);
}

