#include <Arduino.h>
#include <ESP32Servo.h>

int door_pin = 2;
int deploy_pin = 4;
int steer_pin = 5;

int steer_angle = 0;

Servo door;
Servo deploy;
Servo steer;

void setup() {
  door.attach(door_pin);
  deploy.attach(deploy_pin);
  steer.attach(steer_pin);
  Serial.begin(9600);
}

void loop() {
  /*for (steer_angle = 0; steer_angle < 30; steer_angle += 1){
    steer.write(steer_angle);
    Serial.println(steer_angle);
    delay(50);
  }*/
 deploy.write(90);
 door.write(60);
 steer.write(60);
 delay(1000);
 deploy.write(0);
 steer.write(120);
 door.write(0);
 delay(1000);

}
