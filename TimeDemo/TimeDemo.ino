#include <Wire.h>
#include <WiFi.h>
#include "SSD1306Wire.h"
#include <TimeLib.h>
#include "time.h"

#define uS_TO_S_FACTOR 10000000ULL

SSD1306Wire display(0x3c, SDA, SCL);

const char* ssid       = "WeWork";
const char* password   = "P@ssw0rd";

int hoursreal;
int minutesreal;
int secondsreal;
String hours;
String minutes;
String seconds;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

struct tm timeinfo;
  
void setup() {
  
  Serial.begin(9600);
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  Serial.println("Screen setup success");

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500); 
    display.clear();
    display.drawString(0,0, "Connecting to wifi...");
    display.display();
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  
}

void loop() {
  
  if(!getLocalTime(&timeinfo)){
    display.clear();
    display.drawString(0,0, "Unable to get time");
    display.display();
  }

  int timetosleep = 56;
  int sleeptime = timetosleep - timeinfo.tm_min;
  
  hours = (String)(timeinfo.tm_hour - 9);
  minutes = (String)timeinfo.tm_min;
  seconds = (String)timeinfo.tm_sec;

  hoursreal = timeinfo.tm_hour - 9;
  minutesreal = timeinfo.tm_min;
  secondsreal = timeinfo.tm_sec;

  if (hoursreal < 10) hours = "0" + hours;
  if (minutesreal < 10) minutes = "0" + minutes;
  if (secondsreal < 10) seconds = "0" + seconds;

  display.clear();
  display.drawString(0,0,hours);
  display.drawString(14,0, ":");
  display.drawString(20,0, minutes);
  display.drawString(34,0, ":");
  display.drawString(40,0, seconds);
  display.display();

  if(timetosleep > timeinfo.tm_min){

  esp_sleep_enable_timer_wakeup(sleeptime * uS_TO_S_FACTOR);
  display.clear();
  display.display();
  esp_deep_sleep_start();
  }

}
