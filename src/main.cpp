#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <ESP32Servo.h>
#include <MPU9250.h>

Adafruit_BMP280 altitude;
MPU9250 imu(Wire, 0x68);

const int buttonPin = 35;
bool buttonPressed = false;
unsigned long buttonPressTime = 0;
float zeroPressure = 101325.0;

void print_roll_pitch_yaw()
{
  if (imu.readSensor())
  {
    float accelX = imu.getAccelX_mss();
    float accelY = imu.getAccelY_mss();
    float accelZ = imu.getAccelZ_mss();

    // Calculate pitch and roll
    float pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0 / PI;
    float roll = atan2(accelY, accelZ) * 180.0 / PI;

    // Calculate heading (yaw) using magnetometer data
    float magX = imu.getMagX_uT();
    float magY = imu.getMagY_uT();
    float heading = atan2(magY, magX) * 180.0 / PI;
    if (heading < 0)
    {
      heading += 360.0;
    }

    Serial.print("Yaw: ");
    Serial.println(heading, 2);
    Serial.print("Pitch: ");
    Serial.println(pitch, 2);
    Serial.print("Roll: ");
    Serial.println(roll, 2);
    Serial.println();
  }
}

void print_pressure_temperature()
{
  Serial.print("Pressure: ");
  Serial.println(altitude.readPressure() / 100);
  Serial.print("Temperature: ");
  Serial.println(altitude.readTemperature());
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(buttonPin, INPUT_PULLUP);
  delay(2000);

  Serial.println(F("Altitude Sensor Test"));
  if (!altitude.begin(0x76))
  {
    while (1)
    {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
      delay(500);
    }
  }
  else
  {
    Serial.println(F("BMP280 sensor found"));
  }

  if (!imu.begin())
  {
    while (1)
    {
      Serial.println("MPU9250 connection failed. Please check your connection.");
      delay(500);
    }
  }
  else
  {
    Serial.println(F("MPU9250 sensor found"));
  }
  Serial.println("Calibrating IMU...");
  imu.calibrateAccel();
  Serial.println("Calibrating Gyro...");
  imu.calibrateGyro();
  // Serial.println("Calibrating Mag...");
  // imu.calibrateMag();
}

void loop()
{
  print_roll_pitch_yaw();
  print_pressure_temperature();

  if (digitalRead(buttonPin) == LOW)
  {
    if (!buttonPressed)
    {
      buttonPressed = true;
      buttonPressTime = millis();
    }
    else if (millis() - buttonPressTime >= 1000)
    {
      zeroPressure = altitude.readPressure();
      Serial.print("Zero pressure set to: ");
      Serial.println(zeroPressure);
      Serial.println();
      buttonPressed = false;
    }
  }
  else
  {
    buttonPressed = false;
    Serial.print("Altitude: ");
    Serial.println(altitude.readAltitude(zeroPressure / 100));
    Serial.println();
  }

  delay(1000);
}
