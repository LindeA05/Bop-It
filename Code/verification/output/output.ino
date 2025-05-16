int ledGroen = 5;
int ledRood = 4;
void setup() {
  Serial.begin(9600);
  pinMode(ledGroen, OUTPUT);
  pinMode(ledRood, OUTPUT);
}

void loop() {
  digitalWrite(ledGroen, HIGH);
  digitalWrite(ledRood, HIGH);
  delay(1000);
  digitalWrite(ledGroen, LOW);
  digitalWrite(ledRood, LOW);
  delay(1000);
}
