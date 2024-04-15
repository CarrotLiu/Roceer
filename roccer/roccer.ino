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
  delay(2000);
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
  if(abs(soccerReadings.accX) < 3.6 && abs(soccerReadings.accY) < 3.6){
    analogWrite(motor1IN, 0);
    analogWrite(motor1OUT, 0);
    analogWrite(motor2IN, 0);
    analogWrite(motor2OUT, 0);
  }else{
    if(abs(soccerReadings.accX) > abs(soccerReadings.accY)){
      if(soccerReadings.accX > 0){
        float spdX = map(soccerReadings.accX, 0, 50, 160, 255);
        analogWrite(motor1IN, spdX);
        analogWrite(motor1OUT, 0);
        analogWrite(motor2IN, 0);
        analogWrite(motor2OUT, spdX);
      }else{
        float spdX = map(soccerReadings.accX, -50, 0, 255, 160);
        analogWrite(motor1IN, 0);
        analogWrite(motor1OUT, spdX);
        analogWrite(motor2IN, spdX);
        analogWrite(motor2OUT, 0);
      }
    }else{
      if(soccerReadings.accY > 0){
        float spdY = map(soccerReadings.accY, 0, 50, 160, 255);
        analogWrite(motor1IN, spdY);
        analogWrite(motor1OUT, 0);
        analogWrite(motor2IN, spdY);
        analogWrite(motor2OUT, 0);
      }else if(soccerReadings.accY < 0){
        float spdY = map(soccerReadings.accY, -50, 0, 255, 160);
        analogWrite(motor1IN, 0);
        analogWrite(motor1OUT, spdY);
        analogWrite(motor2IN, 0);
        analogWrite(motor2OUT, spdY);    
      }
    }
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&soccerReadings, incomingData, sizeof(soccerReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
}
