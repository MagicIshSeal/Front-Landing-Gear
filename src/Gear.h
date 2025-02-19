#ifndef GEAR_H
#define GEAR_H

#include <ESP32Servo.h>
#include <ServoEasing.hpp>

class Gear
{
private:
    ServoEasing door;   // Servo for the door
    ServoEasing deploy; // Servo for deploying the gear

    int door_pin, deploy_pin;       // Pins for the servos
    int doorOpenPos, doorClosedPos; // Positions for the door servo
    int gearUpPos, gearDownPos;     // Positions for the deploy servo

    unsigned long interval; // Interval between operations
    bool isDeployed;        // State variable to track gear position

    int doorSpeed;   // Speed for door servo movement
    int deploySpeed; // Speed for deploy servo movement

    enum GearState
    {
        IDLE,
        OPENING_DOOR,
        DEPLOYING_GEAR,
        RETRACTING_GEAR,
        CLOSING_DOOR
    };

    GearState state;
    unsigned long lastActionTime;

    // Function to start moving servo smoothly using ServoEasing library
    void startMoveServoSmoothly(ServoEasing &servo, int endPos, int speed)
    {
        servo.setSpeed(speed);
        servo.startEaseTo(endPos);
    }

public:
    // Constructor to initialize the Gear class
    Gear(int door_pin, int deploy_pin, int doorSpeed = 100, int deploySpeed = 100)
        : door_pin(door_pin), deploy_pin(deploy_pin), interval(1500), isDeployed(false), state(IDLE), lastActionTime(0), doorSpeed(doorSpeed), deploySpeed(deploySpeed) {}

    // Initializes the servos and sets the initial positions
    void init()
    {
        Serial.print("Initializing servos on pins: ");
        Serial.print(door_pin);
        Serial.print(", ");
        Serial.println(deploy_pin);

        door.attach(door_pin);
        deploy.attach(deploy_pin);

        doorOpenPos = 60;
        doorClosedPos = 0;
        gearUpPos = 5;
        gearDownPos = 90;

        up(); // Start with the gear retracted
    }

    // Starts the process to deploy the gear
    void down()
    {
        if (!isDeployed && state == IDLE)
        {
            Serial.println("Deploying gear...");
            state = OPENING_DOOR;
            lastActionTime = millis();
        }
    }

    // Starts the process to retract the gear
    void up()
    {
        if (isDeployed && state == IDLE)
        {
            Serial.println("Retracting gear...");
            state = RETRACTING_GEAR;
            lastActionTime = millis();
        }
    }

    // Update function to be called in the main loop
    void update()
    {
        unsigned long currentTime = millis();

        switch (state)
        {
        case IDLE:
            // Do nothing
            break;

        case OPENING_DOOR:
            if (currentTime - lastActionTime >= interval)
            {
                Serial.print(">door position: ");
                Serial.println(doorOpenPos);
                startMoveServoSmoothly(door, doorOpenPos, doorSpeed); // Start opening the door smoothly
                lastActionTime = currentTime;
                state = DEPLOYING_GEAR;
            }
            break;

        case DEPLOYING_GEAR:
            if (currentTime - lastActionTime >= interval)
            {
                Serial.print(">gear position: ");
                Serial.println(gearDownPos);
                startMoveServoSmoothly(deploy, gearDownPos, deploySpeed); // Start deploying the gear smoothly
                lastActionTime = currentTime;
                state = IDLE;
                isDeployed = true;
                Serial.println("Gear deployed.");
            }
            break;

        case RETRACTING_GEAR:
            if (currentTime - lastActionTime >= interval)
            {
                Serial.print(">gear position: ");
                Serial.println(gearUpPos);
                startMoveServoSmoothly(deploy, gearUpPos, deploySpeed); // Start retracting the gear smoothly
                lastActionTime = currentTime;
                state = CLOSING_DOOR;
            }
            break;

        case CLOSING_DOOR:
            if (currentTime - lastActionTime >= interval)
            {
                Serial.print(">door position: ");
                Serial.println(doorClosedPos);
                startMoveServoSmoothly(door, doorClosedPos, doorSpeed); // Start closing the door smoothly
                lastActionTime = currentTime;
                state = IDLE;
                isDeployed = false;
                Serial.println("Gear retracted and door closed.");
            }
            break;
        }

        // Update the servos to ensure smooth movement
        door.update();
        deploy.update();
    }
};

#endif // GEAR_H