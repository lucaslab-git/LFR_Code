#include "Motor.h"
#include "Arduino.h"

Motor::Motor(int mounting_deg, u_int8_t pin_fw, u_int8_t pin_bw, int friction_tuning)
{
    mounting_offset = mounting_deg;
    friction_offset = friction_tuning;
    p_fw = pin_fw;
    p_bw = pin_bw;
    pinMode(p_fw, OUTPUT);
    pinMode(p_bw, OUTPUT);
}

Motor::~Motor()
{
}

void Motor::drive_direction(int degree)
{
    int speed = 130 * sin((PI / 180) * (degree - mounting_offset)); // 255 = speed

    // Serial.print(" Motor speed: ");
    // Serial.print(speed);
    if (speed > 0)
    {
        Serial.println("speed:" + String(speed + 50 + friction_offset));
        analogWrite(p_fw, speed + 50 + friction_offset);
        analogWrite(p_bw, 0);
    }
    else if (speed < 0)
    {
        Serial.println("abs:" + String(abs(speed - 50 - friction_offset)));
        analogWrite(p_fw, 0);
        analogWrite(p_bw, abs(speed - 50 - friction_offset));
    }
    else 
    {
        Serial.println("speed: 0");
        analogWrite(p_fw, 0);
        analogWrite(p_bw, 0);
    }
};

void Motor::stop()
{
    Serial.println("stoped");
    analogWrite(p_fw, 0);
    analogWrite(p_bw, 0);
};

void Motor::drive_speed(int speed, dir pdir){

};
void Motor::drive_around_obstacle(side pside){

};
