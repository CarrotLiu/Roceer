#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <esp_now.h>
#include "WiFi.h"

#include <Wire.h>

Adafruit_MPU6050 mpu;

float aX, aY, aZ;

uint8_t broadcastAddress[] = {0x08, 0xB6, 0x1F, 0xB9, 0x55, 0xCC}; // Receiver address

typedef struct struct_message {
    float accX;
    float accY;
    float accZ;
} struct_message;

struct_message controllerReadings;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_MODE_STA);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void getReadings() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  aX = a.acceleration.x;
  aY = a.acceleration.y;
  aZ = a.acceleration.z;
}

void loop() {
  getReadings();

  controllerReadings.accX = aX;
  controllerReadings.accY = aY;
  controllerReadings.accZ = aZ;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &controllerReadings, sizeof(controllerReadings));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}
