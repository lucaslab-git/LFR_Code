#include "Motor.h"
#include "Arduino.h"

Motor::Motor(int mounting_deg, int pin_fw, int pin_bw)
{
    // pinMode(Output) wenn nicht in main setup
    Serial.println(mounting_deg);
    Serial.println(pin_fw);
    Serial.println(pin_bw);
    mounting_offset = mounting_deg;
    p_fw = pin_fw;
    p_bw = pin_bw;
}

Motor::~Motor()
{
}

void Motor::drive_direction(int degree)
{

    int speed = 255 * sin((PI / 180) * (degree - mounting_offset)); // 255 = speed

    // Serial.print(" Motor speed: ");
    // Serial.print(speed);
    if (speed <= 0)
    {
        analogWrite(p_fw, speed);
        analogWrite(p_bw, 0);
    }
    else
    {
        analogWrite(p_fw, 0);
        analogWrite(p_bw, abs(speed));
    }
};

void Motor::stop()
{
    analogWrite(p_fw, 0);
    analogWrite(p_bw, 0);
};

void Motor::drive_speed(int speed, dir pdir){

};
void Motor::drive_around_obstacle(side pside){

};
