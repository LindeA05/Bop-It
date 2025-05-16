/* Linde Aerssens en Janne Swijsen
    opkomende technologiën --> bop it
    enkel spel
*/

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
bool actie = false;


// knoppen en lichtjes
int buttonRestart = 9;
int buttonPush = 7;
int buttonPull = 2;
int buttonTwist = 3;
int ledRood = 6;
int ledGroen = 5;
int lastTime = 0;
int reactieTijd = 5000;

 
void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);

  if (!myDFPlayer.begin(mySerial)) {
        Serial.println("DFPlayer Mini not detected!");
        while (true);
    }
    

  pinMode(buttonRestart, INPUT_PULLUP);

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
 


if (digitalRead(buttonRestart) == LOW) {
  Serial.println("--Restart--");
   delay(1000);
   inHetSpel = true;
    counter = 0;
    level = 0;

}
}

void correct(){
digitalWrite(ledGroen, HIGH);

   counter++;

    Serial.println("Goed!  ");
  
  Serial.println(counter);
  
  delay(1000);

    digitalWrite(ledGroen, LOW);
    
    if (counter >= 3) {

      counter = 0;

      level = level + 1;

      reactieTijd = reactieTijd - 1500;

      Serial.print("nu in level:");
      Serial.println(level);
     

}
if (level == 4) {

inHetSpel = false;

Serial.println("je hebt gewonnen!!");
}


}

void fout(){
  digitalWrite(ledRood, HIGH);

    Serial.println("Fout!");

    delay(1000);

    digitalWrite(ledRood, LOW);
    inHetSpel = false;

    Serial.println("Je zat in level: ");
    Serial.print(level);
    Serial.print(" met een score van: ");
    Serial.print(counter);
}

void bopIt() {
  Serial.println("bopIt");

  unsigned long startTijd = millis();

 
 myDFPlayer.volume(25);  
 myDFPlayer.play(3);      


  while (millis() - startTijd < reactieTijd) {
      actie = false;
    if (digitalRead(buttonPush) == LOW) {

      correct();
      actie = true;
      break;
  }
      
     if ((digitalRead(buttonPull) == LOW)||(digitalRead(buttonTwist) == LOW)) {

      fout();
      actie = true;

      break;

    }
  }
  if (actie){ 
  }else {
    fout();
  }

  delay(reactieTijd);

}

void pullIt() {

  Serial.println("pullIt");

  unsigned long startTijd = millis();
  
  
  myDFPlayer.volume(25);  
  myDFPlayer.play(2);      

  while (millis() - startTijd < reactieTijd) {
    actie = false;
    if (digitalRead(buttonPull) == LOW) {

      correct();
      actie = true;

      break;
      
      }
    
     if ((digitalRead(buttonPush) == LOW)||(digitalRead(buttonTwist) == LOW)) {
      
    fout();
    actie = true;

      break;

    }

  }
  if (actie){ 
  }else {
    fout();
  }
  delay(reactieTijd);

}

void twistIt() {

  Serial.println("twistIt");

  unsigned long startTijd = millis();

  
  myDFPlayer.volume(25);  
  myDFPlayer.play(1);      


  while (millis() - startTijd < reactieTijd) {
    actie = false;
    if (digitalRead(buttonTwist) == LOW) {

      correct();
      actie = true;

      break;
      } 

   
     if ((digitalRead(buttonPush) == LOW)||(digitalRead(buttonPull) == LOW)) {

      fout();
      actie = true;
      break;

    }

  }
  if (actie){ 
  }else {
    fout();
  }
  delay(reactieTijd);

}
