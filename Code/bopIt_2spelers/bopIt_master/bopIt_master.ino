// Linde Aerssens en Janne Swijsen


#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//topics
const char broker[] = "broker.hivemq.com";
int        port     = 1883;
const char actie[]  = "real_unique_actie";
const char reactie[]  = "real_unique_reactie";
const char resultaatSpeler1[]  = "real_unique_resultaatSpeler1";
const char resultaatSpeler2[]  = "real_unique_resultaatSpeler2";
const char winnaar[]  = "real_unique_winnaar";

int score1 = 0;
int score2 = 0;
bool actieGestart = false;
bool actieBezig = false;
unsigned long startTijd;
String huidigeActie = "";

int buttonPush = 6;
int buttonPull = 7;
int buttonTwist = 8;
int ledGroen = 5;
int ledRood = 4;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  // ontvangen
  mqttClient.onMessage(onMqttMessage);
  mqttClient.subscribe(reactie); 
  // pinnen
  pinMode(buttonPush, INPUT_PULLUP);
  pinMode(buttonPull, INPUT_PULLUP);
  pinMode(buttonTwist, INPUT_PULLUP);
  pinMode(ledGroen, OUTPUT);
  pinMode(ledRood, OUTPUT);
}

void loop() {
  mqttClient.poll();
  
// actiekiezen of de input contreleren
  if (!actieGestart && !actieBezig) {
     
    kiesActie();
  } else {
    controleerInput("speler1");
  }
}
// random actie kiezen en doorsturen naar slave
void kiesActie() {
  int keuze = random(1, 4);
  if (keuze == 1) {
    huidigeActie = "bopIt";
  }
  else if (keuze == 2){
    huidigeActie = "pullIt";
  } else {
    huidigeActie = "twistIt";
  }

  mqttClient.beginMessage(actie);
  mqttClient.print(huidigeActie); // Moet dit huidigeAcite.c_str zijn??
  mqttClient.endMessage();
  Serial.print("Nieuwe actie: "); 
  Serial.println(huidigeActie);
  actieGestart = true;
  startTijd = millis();
}
// input controleren en doorsturen
void controleerInput(String speler) {
  bool correct = false;
  if (huidigeActie == "bopIt" && digitalRead(buttonPush) == LOW) {
    correct = true;
  }
  else if (huidigeActie == "pullIt" && digitalRead(buttonPull) == LOW) {
    correct = true;
  }
  else if (huidigeActie == "twistIt" && digitalRead(buttonTwist) == LOW){
    correct = true;
  }

  if (correct) {
    mqttClient.beginMessage(reactie);
    mqttClient.print("speler1");
    mqttClient.endMessage();
    
    
  }else{
    
  }

  
}

void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String bericht = "";
  while (mqttClient.available()) {
    bericht += (char)mqttClient.read();
  }
  if (topic == reactie) {
    if (!actieGestart) return;

    actieGestart = false;
//aangeven wie het eerst op de knop heeft gedrukt
    if (bericht == "speler1") {
      score1++;
      Serial.print("Score1:");
      Serial.println(score1);
      Serial.print("Score2:");
      Serial.println(score2);
      mqttClient.beginMessage(resultaatSpeler1);
      mqttClient.print("goed");
      mqttClient.endMessage();
      mqttClient.beginMessage(resultaatSpeler2);
      mqttClient.print("fout");
      mqttClient.endMessage();
      digitalWrite(ledGroen, HIGH);
      delay(1000);
      digitalWrite(ledGroen, LOW);
    } else if (bericht == "speler2") {
      score2++;
      Serial.print("Score1:");
      Serial.println(score1);
      Serial.print("Score2:");
      Serial.println(score2);
      mqttClient.beginMessage(resultaatSpeler2);
      mqttClient.print("goed");
      mqttClient.endMessage();
      mqttClient.beginMessage(resultaatSpeler1);
      mqttClient.print("fout");
      mqttClient.endMessage();
      digitalWrite(ledRood, HIGH);
      delay(1000);
      digitalWrite(ledRood, LOW);
    }
//winnaar doorsturen en aangeven
    if (score1 >= 10) {
      score1 = 0;
      score2 =0;
      mqttClient.beginMessage(winnaar);
      mqttClient.print("speler1");
      mqttClient.endMessage();
      Serial.println("Jij wint!");
      for (int i = 0; i < 5; i++) {
        digitalWrite(ledGroen, HIGH); delay(200);
        digitalWrite(ledGroen, LOW); delay(200);
      }
    }
    else if (score2 >= 10) {
      score1=0;
      score2 =0;
      mqttClient.beginMessage(winnaar);
      mqttClient.print("speler2");
      mqttClient.endMessage();
    actieBezig = false;
    }
  }
}


//void callback(char* topic, byte* payload, unsigned int length) {
  //String bericht;
  //for (int i = 0; i < length; i++){
    //bericht += (char)payload[i];
  //}
  
  //if (topic == reactie) {
    //if (!actieGestart) return;

    //actieGestart = false;

    //if (bericht == "speler1") {
      //score1++;
      //Serial.print("Score1");
      //Serial.println(score1);
      //Serial.print("Score2:");
      //Serial.println(score2);
      //mqttClient.beginMessage(resultaatSpeler1);
      //mqttClient.print("goed");
      //mqttClient.endMessage();
      //mqttClient.beginMessage(resultaatSpeler2);
      //mqttClient.print("fout");
      //mqttClient.endMessage();
      //digitalWrite(ledGroen, HIGH);
      //delay(1000);
      //digitalWrite(ledGroen, LOW);
    //} else if (bericht == "speler2") {
      //score2++;
      //Serial.print("Score1:");
      //Serial.println(score1);
      //Serial.print("Score2:");
      //Serial.println(score2);
      //mqttClient.beginMessage(resultaatSpeler2);
      //mqttClient.print("goed");
      //mqttClient.endMessage();
      //mqttClient.beginMessage(resultaatSpeler1);
      //mqttClient.print("fout");
      //mqttClient.endMessage();
      //digitalWrite(ledRood, HIGH);
      //delay(1000);
      //digitalWrite(ledRood, LOW);
    //}

    //if (score1 >= 10) {
      //mqttClient.beginMessage(winnaar);
      //mqttClient.print("speler1");
      //mqttClient.endMessage();
      //Serial.println("🎉 Jij wint!");
      //for (int i = 0; i < 5; i++) {
        //digitalWrite(ledGroen, HIGH); delay(200);
        //digitalWrite(ledGroen, LOW); delay(200);
      //}
    //}
    //else if (score2 >= 10) {
      //mqttClient.beginMessage(winnaar);
      //mqttClient.print("speler2");
      //mqttClient.endMessage();
    //}
  //}
//}

//void reconnect() {
  //while (!client.connected()) {
    //if (client.connect("speler1")) {
      //client.subscribe("game/reactie");
    //} else {
      //delay(1000);
    //}
  //}
//}

