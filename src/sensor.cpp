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

  Serial.println(F("Altitude Sensor Test"));
  if (!altitude.begin())
  {
    while (1)
    {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
      delay(10);
    }
  }

  Serial.println(F("Accelerometer Test"));
  if (!accel.setup(0x68))
  {
    while (1)
    {
      Serial.println(F("Could not find a valid MPU9250 sensor, check wiring or try a different address!"));
      delay(10);
    }
  }

  delay(1000);
  // Code to calibrate the sensors
  /*Serial.println("Accel Gyro calibration will start in 5sec.");
  Serial.println("Please leave the device still on the flat plane.");
  mpu.verbose(true);
  delay(5000);
  mpu.calibrateAccelGyro();

  Serial.println("Mag calibration will start in 5sec.");
  Serial.println("Please Wave device in a figure eight until done.");
  delay(5000);
  mpu.calibrateMag();

  print_calibration();
  mpu.verbose(false);*/
}

void loop()
{
  printAccelerometer();
  if (altitude.takeForcedMeasurement())
  {
    Serial.print("Pressure: ");
    Serial.print(altitude.readPressure());
    Serial.println(" Pa");
    Serial.print("Temperature: ");
    Serial.print(altitude.readTemperature());
    Serial.println(" C");
  }
  else
  {
    Serial.println("Force measurement failed");
  }
  delay(1000);
}

void printAccelerometer()
{
  Serial.print("Yaw: ");
  Serial.print(accel.getYaw(), 6);
  Serial.print(" Roll: ");
  Serial.print(accel.getRoll(), 6);
  Serial.print(" Pitch: ");
  Serial.println(accel.getPitch(), 6);
  Serial.print("X Accel: ");
  Serial.println(accel.getAccX(), 6);
  Serial.print("Y Accel: ");
  Serial.println(accel.getAccY(), 6);
  Serial.print("Z Accel: ");
  Serial.println(accel.getAccZ(), 6);
}