#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Replace 0x27 with the address you found using the I2C scanner
LiquidCrystal_I2C lcd(0x27, 16, 2);
byte customChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

void setup() {
  lcd.init();

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0); // Set cursor to the first row, first column
  lcd.print("Hello, World!"); // Print text on the LCD
  lcd.setCursor(0, 1); // Set cursor to the second row, first column
  lcd.print("Plant Project!"); // Print another line of text
  lcd.createChar(0, customChar); // create a new custom character
}

void loop() {
  lcd.backlight(); // Turn on the backlight
  delay(1000); // Wait for a second
  // lcd.noBacklight(); // Turn on the backlight
  delay(1000); // Wait for a second


  // Nothing to do here
}