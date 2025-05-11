#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID
char pass[] = SECRET_PASS;    // your network password

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char actie[]  = "real_unique_actie";
const char reactie[]  = "real_unique_reactie";
const char resultaatSpeler1[]  = "real_unique_resultaatSpeler1";
const char resultaatSpeler2[]  = "real_unique_resultaatSpeler2";
const char winnaar[]  = "real_unique_winnaar";

String huidigeActie = "";
bool actieActief = false;
unsigned long startTijd;

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
  Serial.print("Attempting to connect to SSID: ");
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

//MQTT connectie
  Serial.println("Connecting to MQTT broker...");
  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");

  // set the message receive callback
  mqttClient.onMessage(onMqttMessage);

  // subscribe to a topic
  mqttClient.subscribe(actie);
  mqttClient.subscribe(resultaatSpeler1);
  mqttClient.subscribe(winnaar);

  pinMode(buttonPush, INPUT_PULLUP);
  pinMode(buttonPull, INPUT_PULLUP);
  pinMode(buttonTwist, INPUT_PULLUP);
  pinMode(ledGroen, OUTPUT);
  pinMode(ledRood, OUTPUT);
}

void loop() {
  mqttClient.poll();

  if (actieActief) {
    bool correct = false;
    if (huidigeActie == "bopIt" && digitalRead(buttonPush) == LOW) correct = true;
    else if (huidigeActie == "pullIt" && digitalRead(buttonPull) == LOW) correct = true;
    else if (huidigeActie == "twistIt" && digitalRead(buttonTwist) == LOW) correct = true;

    if (correct) {
      mqttClient.beginMessage(reactie);
      mqttClient.print("speler2");
      mqttClient.endMessage();
      actieActief = false;
    }

    if (millis() - startTijd > 3000) {
      actieActief = false;
    }
  }
}

void onMqttMessage(int messageSize) {
  String topic = mqttClient.messageTopic();
  String bericht = "";

  while (mqttClient.available()) {
    bericht += (char)mqttClient.read();    
  }
  Serial.print("Ontvangen topic: ");
  Serial.println(topic);
  Serial.print("Bericht: ");
  Serial.println(bericht);

  if (topic == actie) {
    huidigeActie = bericht;
    actieActief = true;
    startTijd = millis();
    Serial.print("Actie ontvangen: ");
    Serial.println(huidigeActie);
  }
  if (topic == resultaatSpeler2) {
    if (bericht == "goed") {
      digitalWrite(ledGroen, HIGH);
      delay(1000);
      digitalWrite(ledGroen, LOW);
    } else {
      digitalWrite(ledRood, HIGH);
      delay(1000);
      digitalWrite(ledRood, LOW);
    }
  if (topic == winnaar && bericht == "speler2") {
    Serial.println("🎉 Jij wint!");
    for (int i = 0; i < 5; i++) {
      digitalWrite(ledGroen, HIGH); delay(200);
      digitalWrite(ledGroen, LOW); delay(200);
    }
    
  }
  }
}
  



  