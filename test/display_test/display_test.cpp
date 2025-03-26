#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Replace 0x27 with the address you found using the I2C scanner
LiquidCrystal_I2C lcd(0x27, 16, 2);
byte heartChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte char1[8] = {
  0b00100, // row 1: top of the droplet shape
  0b01110, // row 2: body of the droplet
  0b11111, // row 3: wider bottom of the droplet
  0b01110, // row 4: bottom part of the droplet narrowing
  0b00100, // row 5: narrow point at the bottom of the droplet
  0b01010, // row 6: a wavy line to represent humidity (wave)
  0b10001, // row 7: wavy line continues
  0b01010  // row 8: wavy line continues
};

byte char2[8] = {
  0b00100, // row 1: creates the top round portion of the droplet
  0b01110, // row 2: continues the curve of the droplet
  0b11111, // row 3: middle section of the droplet
  0b11111, // row 4: bottom section still filled
  0b01110, // row 5: bottom narrowing
  0b00100, // row 6: narrow point at the bottom of the droplet
  0b00000, // row 7: empty for the space below
  0b00000  // row 8: empty for the space below
};


byte char3[8] = {
  0b01110, // row 1: top of the droplet, starting to round out
  0b11111, // row 2: widening out more to form the body of the droplet
  0b11111, // row 3: body of the droplet continues
  0b11111, // row 4: body of the droplet stays wide
  0b01110, // row 5: narrowing towards the bottom
  0b00100, // row 6: narrowing even more
  0b00010, // row 7: narrow point at the very bottom
  0b00001  // row 8: point of the droplet, completing the shape
};

byte char4[8] = {
  0b00010, // row 1: start of the wave
  0b00101, // row 2: part of the wave moving upwards
  0b01001, // row 3: continuing the wave
  0b10001, // row 4: the wave reaches the peak
  0b01001, // row 5: wave starts descending
  0b00101, // row 6: continuing the descent
  0b00010, // row 7: wave nearing completion
  0b00000  // row 8: flat line, completing the wave shape
};

byte char5[8] = {
  0b00001, // row 1: starting the first wave
  0b00101, // row 2: forming a rising curve
  0b01001, // row 3: continuing the curve upwards
  0b10001, // row 4: peak of the curve
  0b01001, // row 5: starting to descend
  0b00101, // row 6: continuing the descent
  0b00001, // row 7: completing the wave
  0b00000  // row 8: flat, no further curve
};
byte char6[8] = {
  0b01010, // row 1: small upward curve
  0b10101, // row 2: medium upward curve
  0b01010, // row 3: small curve descending
  0b10101, // row 4: medium curve again
  0b01010, // row 5: small curve
  0b10101, // row 6: medium curve
  0b01010, // row 7: small curve again
  0b00000  // row 8: flat ending
};

byte char7[8] = {
  0b00100, // row 1: start of the sine wave
  0b01010, // row 2: rising to the middle
  0b10001, // row 3: peak of the wave
  0b01010, // row 4: descending
  0b00100, // row 5: trough of the wave
  0b01010, // row 6: rising again
  0b10001, // row 7: second peak
  0b01010  // row 8: ending at a rising middle point
};

byte char8[8] = {
  0b00010, // row 1: start of a line
  0b00101, // row 2: rising line with slight curvature
  0b01001, // row 3: wave peaks slightly
  0b10001, // row 4: peak of the vapor
  0b01001, // row 5: wave descending
  0b00101, // row 6: descending continues
  0b00010, // row 7: line continues
  0b00000  // row 8: flat end, simulating evaporation
};

byte char9[8] = {
  0b00100, // row 1: rising streak starts
  0b01010, // row 2: wave forms, rising
  0b10001, // row 3: peak of the streak
  0b01010, // row 4: wave descends
  0b00100, // row 5: moving back up again
  0b01010, // row 6: rising again
  0b10001, // row 7: second peak
  0b00000  // row 8: vapor dissipating
};

byte char10[8] = {
  0b00000, // row 1: space
  0b00100, // row 2: small "2"
  0b00100, // row 3: "2"
  0b00100, // row 4: "2"
  0b00000, // row 5: space
  0b00000, // row 6: space
  0b00000, // row 7: space
  0b00000  // row 8: space
};

void setup() {
  lcd.init();

  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0); // Set cursor to the first row, first column
  lcd.print("Hello, World!"); // Print text on the LCD
  lcd.setCursor(0, 1); // Set cursor to the second row, first column
  lcd.print("Plant Project!"); // Print another line of text
  lcd.createChar(0, heartChar); // create a new custom character
  lcd.createChar(1, char1); // create a new custom character
  lcd.createChar(2, char2); // create a new custom character
  lcd.createChar(3, char3); // create a new custom character
  lcd.createChar(4, char4); // create a new custom character
  lcd.createChar(5, char5); // create a new custom character
  lcd.createChar(6, char6); // create a new custom character
  lcd.createChar(7, char7); // create a new custom character
  lcd.createChar(8, char8); // create a new custom character
  lcd.createChar(9, char9); // create a new custom character
  lcd.createChar(10, char10); // create a new custom character
  lcd.clear(); // Clear the LCD
}

void loop() {
  for (int i = 0; i < 8; i++) {
    lcd.setCursor(i*2, 0);
    lcd.write(i);

  }
  lcd.backlight(); // Turn on the backlight
  delay(1000); // Wait for a second
  lcd.setCursor(0,1);
  lcd.write(9);
  lcd.setCursor(3,1);
  lcd.write(10);
  lcd.setCursor(5,1);
  lcd.print("H2O");
  // lcd.noBacklight(); // Turn on the backlight
  delay(1000); // Wait for a second


  // Nothing to do here
}