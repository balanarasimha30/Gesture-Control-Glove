#include <Keyboard.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Create accelerometer object
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Flex sensor pins
#define FLEX1 A0
#define FLEX2 A1

// Threshold values (adjust after calibration)
int flex1Threshold = 500;
int flex2Threshold = 520;

// State flags (to prevent repeated triggering)
bool flex1Active = false;
bool flex2Active = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();

  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_16_G);
  Serial.println("Gesture Control System Ready...");
}

void loop() {

  sensors_event_t event;
  accel.getEvent(&event);

  int flex1Value = analogRead(FLEX1);
  int flex2Value = analogRead(FLEX2);
  float x = event.acceleration.x;

  // ---------- FLEX SENSOR 1 (Forward) ----------
  if (flex1Value > flex1Threshold && !flex1Active) {
    Keyboard.write('W');
    flex1Active = true;
  }

  if (flex1Value < flex1Threshold - 30) {
    flex1Active = false;
  }

  // ---------- FLEX SENSOR 2 (Brake) ----------
  if (flex2Value > flex2Threshold && !flex2Active) {
    Keyboard.write('S');
    flex2Active = true;
  }

  if (flex2Value < flex2Threshold - 30) {
    flex2Active = false;
  }

  // ---------- RIGHT TILT ----------
  if (x < -6.0) {
    Keyboard.press('D');
    delay(80);
    Keyboard.releaseAll();
  }

  // ---------- LEFT TILT ----------
  if (x > 6.0) {
    Keyboard.press('A');
    delay(80);
    Keyboard.releaseAll();
  }

  delay(100);
}
