int buttonPush = 6;
int buttonPull = 7;
int buttonTwist = 8;
void setup() {
  Serial.begin(9600);
  pinMode(buttonPush, INPUT_PULLUP);
  pinMode(buttonPull, INPUT_PULLUP);
  pinMode(buttonTwist, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(buttonPush)== LOW){
    Serial.println("Bop It!");
  }
  if (digitalRead(buttonPull) == LOW){
    Serial.println("Pull It!");
  }
  if (digitalRead(buttonTwist) == LOW){
    Serial.println("Twist It!");
  }
}
