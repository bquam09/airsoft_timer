/*
Code from this website:
https://mechatrofice.com/arduino/arduino-countdown-lcd-display-in-hhmmss-format
*/

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
String action;
long timein[6], countdown_time = 0, initialsecond = 0;
int i = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("HH:MM:SS");
  lcd.setCursor(4, 1);
  lcd.print("00:00:00");
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    switch (key) {
      //If key is C set time.
      case 'C'  :
        action = "set_time";
        lcd.setCursor(4, 1);
        lcd.blink();
        i = 0;
        break;
      //If key is D start countdown.
      case 'D'  :
        action  = "start_countdown";
        lcd.noBlink();
        break;
      default :
        if (action == "set_time") {
          i++;
          int c = i - 1;
          timein[c] = key - 48;
          initialsecond = 0;
          long hour = (timein[0] * 10) + timein[1];
          long minute = (timein[2] * 10) + timein[3];
          long second = (timein[4] * 10) + timein[5]; //second
          countdown_time = (hour * 3600) + (minute * 60) + second;
          lcd.print(key);
          if (i % 2 == 0 && i < 6) {
            lcd.print(":");
          }
          break;
        }
    }
  }
  if (action  == "start_countdown") {
    if (initialsecond == 0) {
      initialsecond = millis() / 1000;
    }
    long countdowntime_seconds = countdown_time - (millis() / 1000) + initialsecond;
    if (countdowntime_seconds >= 0) {
      long countdown_hour = countdowntime_seconds / 3600;
      long countdown_minute = ((countdowntime_seconds / 60) % 60);
      long countdown_sec = countdowntime_seconds % 60;
      lcd.setCursor(4, 1);
      if (countdown_hour < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_hour);
      lcd.print(":");
      if (countdown_minute < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_minute);
      lcd.print(":");
      if (countdown_sec < 10) {
        lcd.print("0");
      }
      lcd.print(countdown_sec);
    }
    delay(500);
  }
}