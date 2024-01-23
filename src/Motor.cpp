#include "Motor.h"
#include "Arduino.h"


Motor::Motor(int val)
{
    Serial.println("Hello from Motor & val: ");
}

Motor::~Motor()
{
}

void Motor::drive_direction(int degree){

};

void Motor::stop(){

};

void Motor::drive_speed(int speed, dir pdir){

};
void Motor::drive_around_obstacle(side pside){

};
