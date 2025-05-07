// Linde Aerssens en Janne Swijsen


// geluid afspelen

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// verschillende variabelen
long counter = 0;
long level = 0;
int picker;
bool inHetSpel = true;

// knoppen en lichtjes
int buttonPush = 7;
int buttonPull = 2;
int buttonTwist = 3;
int ledRood = 6;
int ledGroen = 5;
int lastTime = 0;
int reactieTijd = 5000;
int startLamp;
 
void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);

  if (!myDFPlayer.begin(mySerial)) {
        Serial.println("DFPlayer Mini not detected!");
        while (true);
    }
    
  
  pinMode(buttonPush, INPUT_PULLUP);

  pinMode(buttonPull, INPUT_PULLUP);

  pinMode(buttonTwist, INPUT_PULLUP);

  pinMode(ledGroen, OUTPUT);

  pinMode(ledRood, OUTPUT);

}


void loop() {

  while (inHetSpel == true) {

    picker = random(1, 4);

    switch (picker) {

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


   
  }

}

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

if (counter >= 10) {

      counter = 0;

      level = level + 1;

      reactieTijd = reactieTijd - 1500;

      Serial.println(level);

      //lose = true;

  }


if (level == 4) {

inHetSpel = false;

Serial.println("je hebt gewonnen!!");
}  

  if (correct) {

    digitalWrite(ledGroen, HIGH);

   counter++;

    Serial.println("Goed!  ");
  
  Serial.print(counter);
    
    
    //Serial.println(counter);

    delay(1000);

    digitalWrite(ledGroen, LOW);

  } else {

    digitalWrite(ledRood, HIGH);

    Serial.println("Fout!");

    delay(1000);

    digitalWrite(ledRood, LOW);

  }

  delay(reactieTijd);

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

  delay(reactieTijd);

}

void twistIt() {

  Serial.println("twistIt");

  unsigned long startTijd = millis();

  bool correct = false;

  Serial.println("DFPlayer Mini ready!");
  myDFPlayer.volume(25);  // Set volume (0 to 30)
  Serial.println("Playing File 001.mp3");
  myDFPlayer.play(1);      // Play first MP3 file


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

  delay(reactieTijd);

}
