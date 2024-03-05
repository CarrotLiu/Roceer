int motor1IN = 14;
int motor1OUT = 27;
int motor2IN = 25;
int motor2OUT = 26;
int spd = 10;
#include "WiFi.h"
void setup(){
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress()); //08:B6:1F:B9:55:CC
  //motor initialization
  pinMode(12, OUTPUT);
  pinMode(motor1IN, OUTPUT);
  pinMode(motor1OUT, OUTPUT);
  digitalWrite(12, HIGH);
}

void loop() {
  digitalWrite(motor1IN, HIGH);
  digitalWrite(motor1OUT, LOW);
  digitalWrite(motor2IN, LOW);
  digitalWrite(motor2OUT, HIGH);
  delay(3000);
  digitalWrite(motor2IN, HIGH);
  digitalWrite(motor2OUT, LOW);
  digitalWrite(motor1IN, LOW);
  digitalWrite(motor1OUT, HIGH);
  delay(3000);
}