#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <MPU9250.h>
#include <FRPPMReceiverSensor.h>
#include "Gear.h"

// Define the pins
const int buttonPin = 35;
bool buttonPressed = false;
unsigned long buttonPressTime = 0;
float zeroPressure = 101325.0;
const int ppmPin = 4;
const int numChan = 10;
const int lgSwitchChan = 4;

// Create the objects
Gear gear(26, 27);
PPMReceiverSensor rx(ppmPin, numChan);
Adafruit_BMP280 altitude;
MPU9250 imu(Wire, 0x68);

void print_roll_pitch_yaw()
{
  if (imu.readSensor())
  {
    float accelX = imu.getAccelX_mss();
    float accelY = imu.getAccelY_mss();
    float accelZ = imu.getAccelZ_mss();
    float roll = 0;

    // Calculate pitch and roll
    float pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180.0 / PI;
    float rollraw = atan2(accelY, accelZ) * 180.0 / PI;
    if (rollraw < 0)
    {
      roll = abs(rollraw) - 180;
    }
    else if (rollraw > 0)
    {
      roll = 180 - rollraw;
    }
    else
    {
      roll = rollraw;
    }

    // Calculate heading (yaw) using magnetometer data
    float magX = imu.getMagX_uT();
    float magY = imu.getMagY_uT();
    float heading = atan2(magY, magX) * 180.0 / PI;
    if (heading < 0)
    {
      heading += 360.0;
    }

    Serial.print(">Yaw: ");
    Serial.println(heading, 2);
    Serial.print(">Pitch: ");
    Serial.println(pitch, 2);
    Serial.print(">Roll: ");
    Serial.println(roll, 2);
    Serial.println();
  }
}

void print_pressure_temperature()
{
  Serial.print(">Pressure: ");
  Serial.println(altitude.readPressure() / 100);
  Serial.print(">Temperature: ");
  Serial.println(altitude.readTemperature());
  Serial.println();
}

void handle_button_press()
{
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
    Serial.print(">Altitude: ");
    Serial.println(altitude.readAltitude(zeroPressure / 100));
    Serial.println();
  }
}

void sensorCheck()
{
  Serial.println(F("Checking sensor availability"));
  delay(100);
  if (!imu.begin())
  {
    while (1)
    {
      Serial.println(F("MPU9250 connection failed. Please check your connection."));
      delay(500);
    }
  }
  else
  {
    Serial.println(F("MPU9250 sensor found"));
  }

  if (!altitude.begin(0x76))
  {
    while (1)
    {
      Serial.println(F("BMP280 connection failed. Please check your connection."));
      delay(500);
    }
  }
  else
  {
    Serial.println(F("BMP280 sensor found"));
  }
}

void checkLandingGear()
{
  int lgSwitchState = rx.ReadChannel(lgSwitchChan);
  if (lgSwitchState > 1500)
  { // 1500 is threshold for switch, change upon testing
    gear.down();
  }
  else
  {
    gear.up();
  }
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  rx.Init();
  pinMode(buttonPin, INPUT_PULLUP);
  gear.init();
  delay(100);

  sensorCheck();

  Serial.println("Calibrating IMU...");
  imu.calibrateAccel();
  Serial.println("Calibrating Gyro...");
  imu.calibrateGyro();
  Serial.println("Calibrating Complete");
  // Serial.println("Calibrating Mag...");
  // imu.calibrateMag();
}

void loop()
{
  print_roll_pitch_yaw();
  print_pressure_temperature();
  handle_button_press();
  checkLandingGear();
  gear.update();
  delay(500);
}
