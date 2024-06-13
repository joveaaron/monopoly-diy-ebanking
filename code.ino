#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "progmemsetting.h"
#include <Preferences.h>


#define YOFFSET 16

#define WIDTH 96
#define HEIGHT 96 + YOFFSET

#define BTN1 18
#define BTN2 19
#define BTN3 23

Adafruit_SH1107 display = Adafruit_SH1107(WIDTH, HEIGHT, &Wire, -1, 400000, 100000);

Preferences preferences;

bool skipIntro = false;

void setup() {
  //pinMode(16, INPUT_PULLUP);
  //pinMode(17, INPUT_PULLUP);
  //pinMode(18, INPUT_PULLUP);

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  Serial.begin(115200);
  //Serial.println("SH1107 TEST SCRIPT");
  delay(1000); //wait for oled startup
  display.begin(0x3C, true); //0x3C for Wokwi, 0x78 for real display
  display.cp437(true);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  if(!skipIntro) { intro(); }
  display.clearDisplay();
  display.display();
  preferences.begin("monopoly", false);
  checkunfinishedgame();
  configureGame();
  display.clearDisplay();
  displayActionBar(3,"SUE.", "COMUN.", "MENU");
  display.display();

  display.setTextColor(2);
  display.setCursor(0,48 + YOFFSET);
  display.print(properties[17].name);
  display.display();
  delay(1000);
}

void loop() {
  display.drawBitmap(0, 0 + YOFFSET, rfidlogo, 25, 16, 2);
  display.setCursor(26,7 + YOFFSET);
  display.print("SCANNING...");
  display.display();
  delay(500);
}

uint8_t configureGame() {
  uint8_t players = 2;
  display.setCursor(0,0 + YOFFSET);
  display.print(selectplayers);
  displayActionBar(3, "-", "OK", "+");
  display.setTextSize(2);
  display.setCursor(30,64 + YOFFSET);
  display.print("\x3C");
  display.print(String(players));
  display.print("\x3E");
  display.display();
  while(true) {
    if(!digitalRead(BTN1)) {
      players--;
      if(players < 2) {
        players = 2;
      }
      display.fillRect(30, 64 + YOFFSET, 66, 64 + YOFFSET, 0);
      display.setTextSize(1);
      displayActionBar(3, "-", "OK", "+");
      display.setTextSize(2);
      display.setCursor(30,64 + YOFFSET);
      display.print("\x3C");
      display.print(String(players));
      display.print("\x3E");
      display.display();
      delay(250);
    } else if(!digitalRead(BTN2)) {
      delay(250);
      break;
    } else if(!digitalRead(BTN3)) {
      players++;
      if(players > 6) {
        players = 6;
      }
      display.fillRect(30, 64 + YOFFSET, 66, 64 + YOFFSET, 0);
      display.setTextSize(1);
      displayActionBar(3, "-", "OK", "+");
      display.setTextSize(2);
      display.setCursor(30,64 + YOFFSET);
      display.print("\x3C");
      display.print(String(players));
      display.print("\x3E");
      display.display();
      delay(250);
    }
  }
  display.setTextSize(1);
  return players;
}

void displayActionBar(uint8_t action_count, const char* action1, const char* action2, const char* action3) {
  display.setTextColor(0);
  uint8_t action1len = strlen(action1);
  uint8_t action2len = strlen(action2);
  uint8_t action3len = strlen(action3);
  if(action_count > 0) {
    display.fillRect(0, 87 + YOFFSET, 95, 95 + YOFFSET, 1);
  }
  switch(action_count) {
    case 1:
      display.setCursor(48 - ((action1len*6)/2),88 + YOFFSET);
      display.print(action1);
      //for(int i=0; i<256; i++) {
      //  digitalWrite(19, LOW);
      //  digitalWrite(23, HIGH);
      //  digitalWrite(25, LOW);
      //  delay(2);
      //}
      //21
      break;
    case 2:
      display.setCursor(0,88 + YOFFSET);
      display.print(action1);
      display.setCursor(95 - (action2len*6),88 + YOFFSET);
      display.print(action2);
      //for(int i=0; i<256; i++) {
      //  digitalWrite(19, HIGH);
      //  digitalWrite(23, LOW);
      //  digitalWrite(25, HIGH);
      //  delay(2);
      //}
      //19 and 26
      break;
    case 3:
      display.setCursor(0,88 + YOFFSET);
      display.print(action1);
      display.setCursor(48 - ((action2len*6)/2),88 + YOFFSET);
      display.print(action2);
      display.setCursor(95 - (action3len*6),88 + YOFFSET);
      display.print(action3);
      //for(int i=0; i<256; i++) {
      //  digitalWrite(19, HIGH);
      //  digitalWrite(23, HIGH);
      //  digitalWrite(25, HIGH);
      //  delay(2);
      //}
      //19, 21 and 26
      break;
    default:
      break;
  }
  display.setTextColor(1);
}

void displaynoticetext() {
  display.clearDisplay();
  display.setCursor(0, 0 + YOFFSET);
  display.print(notice1);
  display.setCursor(0,8 + YOFFSET);
  display.print(notice2);
  display.setCursor(0,16 + YOFFSET);
  display.print(notice3);
  display.setCursor(0,40 + YOFFSET);
  display.print(notice4);
  display.setCursor(0,72 + YOFFSET);
  display.print(notice5);
  display.setCursor(0,88 + YOFFSET);
  display.print(notice6);
  display.display();
}

void displaynoticeqr() {
  display.clearDisplay();
  display.drawBitmap(0, 0 + YOFFSET, noticeqr, 96, 96, 1);
  display.display();
}

void intro() {
  displaynoticetext();
  delay(5000);
  displaynoticeqr();
  delay(10000);
}

void checkunfinishedgame() {
  if(preferences.getBool("unfinished", false)) {
    display.setCursor(0,0 + YOFFSET);
    display.print(unfgame);
    displayActionBar(2,yes, no, "");
    display.display();
    while(true) {
      if(!digitalRead(BTN1)) {
        Serial.println("press BTN2");
        preferences.putBool("unfinished", false);
        break;
      }
      if(!digitalRead(BTN2)) {
        Serial.println("press BTN1");
        break;
      }
    }
  }
}
