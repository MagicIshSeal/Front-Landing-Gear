#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

MPU9250 accel;
Adafruit_BMP280 altitude;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  altitude.begin();
  delay(1000);
}

void loop()
{
  printAccelerometer();
  if (altitude.takeForcedMeasurement())
  {
    Serial.println("Pressure: ");
    Serial.println(altitude.readPressure());
    Serial.print('Pa');
    Serial.println("Temperature: ");
    Serial.println(altitude.readTemperature());
    Serial.print('C');
  }
  else
  {
    Serial.println("Force measurement failed");
  }
  delay(1000);
}

void printAccelerometer()
{
  Serial.print("  Yaw: ");
  Serial.print(accel.getYaw(), 6);
  Serial.println(" Roll: ");
  Serial.print(accel.getRoll(), 6);
  Serial.println(" Pitch: ");
  Serial.println(accel.getPitch(), 6);
  Serial.println("X Accel: ");
  Serial.println(accel.getAccX(), 6);
  Serial.println("Y Accel: ");
  Serial.println(accel.getAccY(), 6);
  Serial.println("Z Accel: ");
  Serial.println(accel.getAccZ(), 6);
}