#include "WiFi.h"
#include <esp_now.h>

int motor1IN = 14;
int motor1OUT = 27;
int motor2IN = 25;
int motor2OUT = 26;
int spd = 10;

typedef struct struct_message {
    float accX;
    float accY;
    float accZ;
} struct_message;

struct_message soccerReadings;

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_MODE_STA);
  
  pinMode(12, OUTPUT);
  pinMode(motor1IN, OUTPUT);
  pinMode(motor1OUT, OUTPUT);
  pinMode(motor2IN, OUTPUT);
  pinMode(motor2OUT, OUTPUT);
  digitalWrite(12, HIGH);
  
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  Serial.print("accX:");
  Serial.print(soccerReadings.accX);
  Serial.print("accY:");
  Serial.print(soccerReadings.accY);
  Serial.print("accZ:");
  Serial.print(soccerReadings.accZ);
  if(soccerReadings.accX > 0){
    digitalWrite(motor1IN, HIGH);
    digitalWrite(motor1OUT, LOW);
    digitalWrite(motor2IN, LOW);
    digitalWrite(motor2OUT, HIGH);
    delay(100);
  }else if(soccerReadings.accX < 0){
    digitalWrite(motor1IN, LOW);
    digitalWrite(motor1OUT, HIGH);
    digitalWrite(motor2IN, HIGH);
    digitalWrite(motor2OUT, LOW);
    delay(100);
  }
  // if(soccerReadings.ac cY > 0){
  //   digitalWrite(motor1IN, HIGH);
  //   digitalWrite(motor1OUT, LOW);
  //   digitalWrite(motor2IN, HIGH);
  //   digitalWrite(motor2OUT, LOW);
  //   delay(1000);
  //   digitalWrite(motor2IN, HIGH);
  //   digitalWrite(motor2OUT, LOW);
  //   digitalWrite(motor1IN, LOW);
  //   digitalWrite(motor1OUT, HIGH);
  //   delay(3000);
  // }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&soccerReadings, incomingData, sizeof(soccerReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
}
