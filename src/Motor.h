#include "Arduino.h"
enum side
{
    right,
    left
};

enum dir
{
    forward,
    backward
};

class Motor
{
private:
    int mounting_offset;
    u_int8_t p_fw;
    u_int8_t p_bw;

public:
    Motor(int mounting_deg, u_int8_t pin_fw, u_int8_t pin_bw); // pinMode(Output) wenn nicht in main setup
    ~Motor();
    void drive_direction(int degree);
    void stop();
    void drive_speed(int speed, dir pdir);
    void drive_around_obstacle(side pside);
};
