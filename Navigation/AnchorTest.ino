/* Considerations checklist
--------------------------
amount of anchors 2v4         0
MUST BE 2 OR 4
--------------------------
amount of pings 1count=1way   0
NEEDS TO BE ODD NUMBER
--------------------------
send/rec data struc           0

SENDERS ID A1234/B1/P1
TIME PACKAGE SENT

A1 POSITION XY
A2 POSITION XY
A3 POSITION XY
A4 POSITION XY

A1-B1 TOF TOTAL
A2-B1 TOF TOTAL
A3-B1 TOF TOTAL //OPTIONAL4
A4-B1 TOF TOTAL //OPTIONAL4

A1-P1 TOF TOTAL
A2-P1 TOF TOTAL
A3-P1 TOF TOTAL //OPTIONAL4
A4-P1 TOF TOTAL //OPTIONAL4

A1-B1 PING COUNT
A2-B1 PING COUNT
A3-B1 PING COUNT //OPTIONAL4
A4-B1 PING COUNT //OPTIONAL4

A1-P1 PING COUNT
A2-P1 PING COUNT
A3-P1 PING COUNT //OPTIONAL4
A4-P1 PING COUNT //OPTIONAL4
--------------------------
REC A1234 TIME SENT ->
TIME REC - TIME SENT ->
ADD TO A1234-B1 TOF TOTAL
--------------------------
IF A1234 PING COUNT == PING TOTAL->
STOP SENDING PACKAGE TO ADDRESS
--------------------------
ALL A1234-B1/P1 PING COUNT == PING TOTAL->
DIVIDE TOF TOTALS BY PINGTOTAL 
--------------------------
2 ANCHOR:
sqrt((A2X-A1X)^2+(A2Y-A1Y))^2 = A1->A2 LENGTH (C)
A1B1TOF/299792458 = A1->B1 LENGTH (B)
A2B1TOF/299792458 = A2->B1 LENGTH (A)

(B^2+C^2-A^2)/2BC = A1B1COS
B*A1B1COS= B1X
B*SQRT(1-A1B1COS^2)= B1Y

--------------------------
4 ANCHOR:
ABS(A1POS-A2POS) = b
A1B1TOF/299792458
--------------------------
*/

#include <WiFi.h>
#include "time.h"
#include "sntp.h"

const char* ssid = "";
const char* password = "";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

String anchorID = "A0";
float anchorX = 0;
float anchorY = 0;
long long int systemTime = 0;

typedef struct rocinfo {
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
  Serial.begin(115200);
  sntp_set_time_sync_notification_cb(getTime);
  sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void loop() {
  updateTime(); //run only when asked for?
  //send msg
  
  
  //rec msg
  
}

void getTime(struct timeval* t) {
  Serial.println("Got time adjustment from NTP!");
  updateTime();
}

void updateTime() {
  char buffer[50];
  time_t now;
  long usec;
  struct tm timeinfo;
  struct timeval tv;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to update time.");
    return;
  }
  time(&now);
  gettimeofday(&tv, NULL);
  usec = tv.tv_usec;
  sprintf(buffer,"%d%d",now,usec);
  systemTime = atoll(buffer);
}
