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
    int p_fw;
    int p_bw;

public:
    Motor(int mounting_deg, int pin_fw, int pin_bw); // pinMode(Output) wenn nicht in main setup
    ~Motor();
    void drive_direction(int degree);
    void stop();
    void drive_speed(int speed, dir pdir);
    void drive_around_obstacle(side pside);
};
