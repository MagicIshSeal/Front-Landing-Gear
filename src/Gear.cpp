#include <Arduino.h>
#include <ESP32Servo.h>

class Gear {
private:
    Servo door;
    Servo deploy;

  int door_pin, deploy_pin, steer_pin;
  int doorOpenPos, doorClosedPos;
  int gearUpPos, gearDownPos;

public:
  Gear(int door_pin, int deploy_pin)
    : door_pin(door_pin), deploy_pin(deploy_pin) {}

  void init() {
    door.attach(door_pin);
    deploy.attach(deploy_pin);

    doorOpenPos = 60;
    doorClosedPos = 0;
    gearUpPos = 0;
    gearDownPos = 90;
      
    up();
  } 

  void down() {
    door.write(doorOpenPos);
    delay(1500);

    deploy.write(gearDownPos);
    delay(1500);
  }

  void up() {
    deploy.write(gearUpPos);
    delay(1500);

    door.write(doorClosedPos);
    delay(1500);
  }
};

int door_pin = 2;
int deploy_pin = 4;
int steer_pin = 5;

Gear gear(door_pin, deploy_pin);

int steer_angle = 0;
Servo steer;


void setup() {
  gear.init();
  steer.attach(steer_pin);
  steer.write(90);
  Serial.begin(9600);
}

void loop() {
  delay(5000);
  gear.down();
  Serial.println("Gear Down");

  for (steer_angle = 90; steer_angle < 180; steer_angle += 1){
    steer.write(steer_angle);
    delay(50);
  }
  for (steer_angle = 180; steer_angle > 0; steer_angle -= 1){ 
    steer.write(steer_angle);
    delay(50);
  }
  steer.write(90);
  delay(2500);
  gear.up();
  Serial.println("Gear up");

}
