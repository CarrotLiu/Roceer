#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid       = "";
const char* password   = "";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

String anchorID = "A0";
float anchorX = 0;
float anchorY = 0;
long systemTime = 0;

typedef struct rocinfo{
  String senderID = "A0";
  long sendTime = 0;

  float a1X = 0;
  float a1Y = 0;
  float a2X = 0;
  float a2Y = 0;

  //to b2
  long a1b1TOF = 0;
  long a2b1TOF = 0;
  
  int a1b1PGC = 0;
  int a2b1PGC = 0;

  //to p1
  long a1p1TOF = 0;
  long a2p1TOF = 0;
  
  int a1p1PGC = 0;
  int a2p1PGC = 0;
};

rocinfo systemStruct;
rocinfo incomingStruct;

void setup() {
  //

}

void loop() {
  // put your main code here, to run repeatedly:

}

void updateTime()
{
  time_t now;
  long usec;
  struct tm timeinfo;
  struct timeval tv;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to update time.");
    return;
  }
  time(&now);
  gettimeofday(&tv,NULL);
  usec = tv.tv_usec;
  String tempStr = (String(now)+String(usec));
  systemTime = tempStr.toInt();
}
