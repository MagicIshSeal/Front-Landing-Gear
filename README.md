# Front Landing Gear

This project controls the front landing gear of an aircraft model using an ESP32 microcontroller. It includes functionalities for deploying and retracting the landing gear, as well as monitoring pressure, temperature, and IMU (Inertial Measurement Unit) data.

## Features

- Deploy and retract landing gear using a switch
- Smooth servo movements using the ServoEasing library
- Monitor pressure and temperature using the BMP280 sensor
- Monitor roll, pitch, and yaw using the MPU9250 sensor
- Handle button press to set zero pressure for altitude calculation

## Hardware Requirements

- ESP32 microcontroller
- Servo motors for door and gear deployment
- BMP280 sensor for pressure and temperature
- MPU9250 sensor for IMU data
- PPM receiver for switch input
- Button for setting zero pressure

## Software Requirements

- Arduino IDE or PlatformIO
- ESP32Servo library
- ServoEasing library
- Adafruit BMP280 library
- MPU9250 library
- FRPPMReceiverSensor library

## Installation

1. Clone the repository:

   ```sh
   git clone https://github.com/yourusername/Front-Landing-Gear.git
   cd Front-Landing-Gear
   ```

2. Open the project in your preferred IDE (Arduino IDE or PlatformIO).

3. Install the required libraries:

   - ESP32Servo
   - ServoEasing
   - Adafruit BMP280
   - MPU9250
   - FRPPMReceiverSensor

4. Connect the hardware components as per the pin definitions in the code.

5. Upload the code to the ESP32 microcontroller.

## Usage

1. Power on the ESP32 microcontroller.
2. The system will initialize the sensors and servos.
3. Use the switch to deploy or retract the landing gear.
4. Press the button to set zero pressure for altitude calculation.
5. Monitor the serial output for pressure, temperature, roll, pitch, and yaw data.

## Code Overview

### [main.cpp](http://_vscodecontentref_/1)

- Initializes the sensors and servos.
- Contains functions to print roll, pitch, yaw, pressure, and temperature.
- Handles button press to set zero pressure.
- Checks the landing gear switch state and updates the gear position.

### [Gear.h](http://_vscodecontentref_/2)

- Defines the [Gear](http://_vscodecontentref_/3) class for controlling the landing gear.
- Uses the ServoEasing library for smooth servo movements.
- Contains functions to deploy and retract the gear.
- Updates the servo positions in a non-blocking manner.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgements

- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo)
- [ServoEasing](https://github.com/ArminJo/ServoEasing)
- [Adafruit BMP280](https://github.com/adafruit/Adafruit_BMP280_Library)
- [MPU9250](https://github.com/bolderflight/MPU9250)
- [FRPPMReceiverSensor](https://github.com/yourusername/FRPPMReceiverSensor)
