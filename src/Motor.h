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
    /* data */
public:
    Motor(int val);
    ~Motor();
    void drive_direction(int degree);
    void stop();
    void drive_speed(int speed, dir pdir);
    void drive_around_obstacle(side pside);
};
