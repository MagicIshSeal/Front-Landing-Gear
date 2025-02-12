#ifndef GEAR_H
#define GEAR_H

#include <ESP32Servo.h>

class Gear
{
private:
    Servo door;   // Servo for the door
    Servo deploy; // Servo for deploying the gear

    int door_pin, deploy_pin, steer_pin; // Pins for the servos
    int doorOpenPos, doorClosedPos;      // Positions for the door servo
    int gearUpPos, gearDownPos;          // Positions for the deploy servo

    unsigned long previousMillis; // Stores the last time the operation was updated
    unsigned long interval;       // Interval between operations
    bool operationInProgress;     // Flag to indicate if an operation is in progress

    // Enum to define the different operations
    enum Operation
    {
        NONE,
        OPEN_DOOR,
        CLOSE_DOOR,
        DEPLOY_GEAR,
        RETRACT_GEAR
    } currentOperation; // Stores the current operation

public:
    // Constructor to initialize the Gear class
    Gear(int door_pin, int deploy_pin)
        : door_pin(door_pin), deploy_pin(deploy_pin), previousMillis(0), interval(1500), operationInProgress(false), currentOperation(NONE) {}

    // Initializes the servos and sets the initial positions
    void init()
    {
        door.attach(door_pin);
        deploy.attach(deploy_pin);

        doorOpenPos = 60;
        doorClosedPos = 0;
        gearUpPos = 0;
        gearDownPos = 90;

        up(); // Start with the gear retracted
    }

    // Starts the process to deploy the gear
    void down()
    {
        currentOperation = OPEN_DOOR;
        operationInProgress = true;
        previousMillis = millis();
    }

    // Starts the process to retract the gear
    void up()
    {
        currentOperation = RETRACT_GEAR;
        operationInProgress = true;
        previousMillis = millis();
    }

    // Updates the state of the gear based on the current operation and time elapsed
    void update()
    {
        if (operationInProgress)
        {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis >= interval)
            {
                previousMillis = currentMillis;
                switch (currentOperation)
                {
                case OPEN_DOOR:
                    door.write(doorOpenPos);        // Open the door
                    currentOperation = DEPLOY_GEAR; // Next operation: deploy the gear
                    break;
                case CLOSE_DOOR:
                    door.write(doorClosedPos);   // Close the door
                    currentOperation = NONE;     // No more operations
                    operationInProgress = false; // Operation complete
                    break;
                case DEPLOY_GEAR:
                    deploy.write(gearDownPos);     // Deploy the gear
                    currentOperation = CLOSE_DOOR; // Next operation: close the door
                    break;
                case RETRACT_GEAR:
                    deploy.write(gearUpPos);       // Retract the gear
                    currentOperation = CLOSE_DOOR; // Next operation: close the door
                    break;
                case NONE:
                    operationInProgress = false; // No operation in progress
                    break;
                }
            }
        }
    }
};

#endif // GEAR_H