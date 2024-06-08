#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "progmemsetting.h"
#include <Preferences.h>

#define WIDTH 96
#define HEIGHT 96


Adafruit_SH1107 display = Adafruit_SH1107(WIDTH, HEIGHT, &Wire, -1, 1000000, 100000);

bool skipIntro = true;

void setup() {
  pinMode(16, INPUT_PULLUP);
  //Serial.begin(115200);
  //Serial.println("SH1107 TEST SCRIPT");
  delay(1000); //wait for oled startup
  display.begin(0x3C, true); //0x3C for Wokwi, 0x78 for real display
  display.cp437(true);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  if(!skipIntro) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(notice1);
    display.setCursor(0,8);
    display.print(notice2);
    display.setCursor(0,16);
    display.print(notice3);
    display.setCursor(0,40);
    display.print(notice4);
    display.setCursor(0,72);
    display.print(notice5);
    display.setCursor(0,88);
    display.print(notice6);
    display.display();
    delay(5000);
    display.clearDisplay();
    display.drawBitmap(0, 0, noticeqr, 96, 96, 1);
    display.display();
    delay(10000);
  }

  display.clearDisplay();
  displayActionBar(3,"SUE.", "COM\xA3N.", "SIG.");
  display.display();
  
  display.setTextColor(2);
  display.setCursor(0,48);
  display.print(properties[0].name);
  display.display();
  delay(1000);
}

void loop() {
  //display.drawBitmap(0, 0, rfidlogo, 25, 16, 2);
  //display.setCursor(26,7);
  //display.print("SCANNING...");
  //display.display();
  //delay(500);
  if(!digitalRead(16)) {
    display.setTextColor(1);
    display.setCursor(26, 7);
    display.print("PRESSED");
    display.display();
  } else {
    display.setTextColor(0);
    display.setCursor(26, 7);
    display.print("PRESSED");
    display.display();
  }
  delay(10);
}

void displayActionBar(uint8_t action_count, const char* action1, const char* action2, const char* action3) {
  display.setTextColor(0);
  uint8_t action1len = strlen(action1);
  uint8_t action2len = strlen(action2);
  uint8_t action3len = strlen(action3);
  if(action_count > 0) {
    display.fillRect(0, 87, 95, 95, 1);
  }
  switch(action_count) {
    case 1:
      display.setCursor(48 - ((action1len*6)/2),88);
      display.print(action1);
      break;
    case 2:
      display.setCursor(0,88);
      display.print(action1);
      display.setCursor(95 - (action2len*6),88);
      display.print(action2);
      break;
    case 3:
      display.setCursor(0,88);
      display.print(action1);
      display.setCursor(48 - ((action2len*6)/2),88);
      display.print(action2);
      display.setCursor(95 - (action3len*6),88);
      display.print(action3);
      break;
    default:
      break;
  }
  display.setTextColor(1);
}
