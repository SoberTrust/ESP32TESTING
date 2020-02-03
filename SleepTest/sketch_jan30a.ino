#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  5
#include <Wire.h>
#include "SSD1306Wire.h"

SSD1306Wire display(0x3c, SDA, SCL);
RTC_DATA_ATTR int bootCount = 0;

void setup() {
    Serial.begin(9600); 
    display.init();
    display.setFont(ArialMT_Plain_10); 
    display.setTextAlignment(TEXT_ALIGN_LEFT); 
    Serial.println("Screen setup complete");

    display.clear(); 
  if(bootCount == 0){
    bootCount += 1;  
    display.setFont(ArialMT_Plain_16);
    display.drawString(0,0,"   SOBERHELP");
    display.setFont(ArialMT_Plain_10);
    display.drawString(0,16,"     Boot is complete");
    display.drawString(0,26,"  wait 3 s for deep sleep");
  }else{
    display.setFont(ArialMT_Plain_16);
    display.drawString(0,0,"   SOBERHELP");
    display.setFont(ArialMT_Plain_10);
    display.drawString(0,16,"    woken up from sleep");
    display.drawString(0,26,"  wait 3 s for deep sleep");
  }
  display.display();
  
  delay(3000);

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  display.clear();
  display.drawString(0,16,"    going to sleep now");
  display.display();
  delay(1500);
  display.clear();
  display.display();
  esp_deep_sleep_start();
}

void loop() {

}
