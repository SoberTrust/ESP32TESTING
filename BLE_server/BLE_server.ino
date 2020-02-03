#include <Wire.h>
#include "SSD1306Wire.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define _GLIBCXX_USE_CXX11_ABI 0
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

SSD1306Wire display(0x3c, SDA, SCL);    //the display
String boop;                            //string for reading bluetooth data for display
BLECharacteristic *pCharacteristic;     //the ble data

void setup() {
  Serial.begin(9600);         //really important that this is same in serial reader and on swift side

  //OLED DISPLAY INIT
  display.init();               //initiate the display

  display.setFont(ArialMT_Plain_10);    //sets the display font

  //BLE INIT
  BLEDevice::init("SOBERHELP");         //initiate BLE server
  BLEServer *pServer = BLEDevice::createServer();           //BLE server part 2
  BLEService *pService = pServer->createService(SERVICE_UUID);    //same stuff here
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |   //allows the read and write
                                         BLECharacteristic::PROPERTY_WRITE    //for the bluetooth data
                                       );

  pCharacteristic->setValue("Hello World");       //sets default value of bluetooth data
  pService->start();                              //note: its cool that you dont have to convert
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();//the string type for arduino
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);  //more bluetooth setup just like it looks
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();        //makes the device discoveravble
  Serial.println("BLE Setup complete! check in iphone");  //sends serial info about bluetooth

}

void loop() {
  // put your main code here, to run repeatedly:
  display.clear();                                    //all display stuff this clears the display
  display.setTextAlignment(TEXT_ALIGN_LEFT);          //all stuff below just prints stuff out
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,0,"   SOBERHELP");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,16,"    BLE Setup is complete");
  boop = (String)pCharacteristic->getValue().c_str();   //gets the value of ble data and converts to cstr
  display.drawString(0,26, boop);                       //really important to use cstr, String() dont work
  display.display();                                  //this makes everything actually print
  if((String)pCharacteristic->getValue().c_str() == "hello"){
      delay(3000);
      pCharacteristic->setValue("Whats up"); 
  }
}
