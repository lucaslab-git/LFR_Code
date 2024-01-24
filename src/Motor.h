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
    uint8_t p_fw;
    uint8_t p_bw;

public:
    Motor(int mounting_deg, uint8_t pin_fw, uint8_t pin_bw);
    ~Motor();
    void drive_direction(int degree);
    void stop();
    void drive_speed(int speed, dir pdir);
    void drive_around_obstacle(side pside);
};
