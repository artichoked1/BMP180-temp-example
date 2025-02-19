#include <Arduino.h>
#include <Wire.h>

// BMP180 I2C address.  Depends on the wiring of the sensor.
#define BMP180_ADDRESS 0x77

// BMP180 Registers
#define BMP180_REG_CALIBRATION 0xAA
#define BMP180_REG_CONTROL 0xF4
#define BMP180_REG_TEMP 0x2E
#define BMP180_REG_RESULT 0xF6

// Calibration coefficient variables
int16_t AC1, AC2, AC3, B_1, B2, MB, MC, MD; // B1 is a reserved name in Arduino
uint16_t AC4, AC5, AC6;

void setup() {
  Wire.begin();
  // Serial.begin(9600);
  // Serial.println("BMP180 temp test");

  // Request the calibration coefficients
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(BMP180_REG_CALIBRATION); // Callibration data starts at register 0xAA
  Wire.endTransmission();

  // Request should be 22 bytes of data
  Wire.requestFrom(BMP180_ADDRESS, 22);
  while (Wire.available() < 22);  // Wait until all 22 bytes are available

  // Parse the calibration coefficients.
  // Each coefficient is 16-bit so we shift the first byte by 8 bits and OR it with the second byte.
  AC1 = (Wire.read() << 8) | Wire.read();
  AC2 = (Wire.read() << 8) | Wire.read();
  AC3 = (Wire.read() << 8) | Wire.read();
  AC4 = (Wire.read() << 8) | Wire.read();
  AC5 = (Wire.read() << 8) | Wire.read();
  AC6 = (Wire.read() << 8) | Wire.read();
  B_1  = (Wire.read() << 8) | Wire.read();
  B2  = (Wire.read() << 8) | Wire.read();
  MB  = (Wire.read() << 8) | Wire.read();
  MC  = (Wire.read() << 8) | Wire.read();
  MD  = (Wire.read() << 8) | Wire.read();
}

void readTemperature() {
  // Query the temperature
  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(BMP180_REG_CONTROL); // Control register
  Wire.write(BMP180_REG_TEMP); // Start temp measurement
  Wire.endTransmission();

  // Wait for the sensor to complete the measurement
  delay(5);  // 4.5ms is recommended by the datasheet

  Wire.beginTransmission(BMP180_ADDRESS);
  Wire.write(0xF6); // Start reading from the result register
  Wire.endTransmission();
  
  Wire.requestFrom(BMP180_ADDRESS, 2);
  while(Wire.available() < 2);
  int UT = (Wire.read() << 8) | Wire.read();

  // Calculate the true temperature
  long X1 = ((long)UT - (long)AC6) * (long)AC5 >> 15;
  long X2 = ((long)MC << 11) / (X1 + MD);
  long B5 = X1 + X2;
  long trueTemp = (B5 + 8) >> 4;
  float temperatureC = trueTemp / 10.0; // Out comes your temp!

  // Print results
  // Serial.print("Raw Temperature (UT): ");
  // Serial.println(UT);
  // Serial.print("True Temperature: ");
  // Serial.print(trueTemp / 10.0);
  // Serial.println(" °C");
}

void loop() {
  readTemperature();
  delay(1000);
}
