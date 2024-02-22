#include "Camera.h"
enum side
{
    left,
    right,
    up,
    down,
    none
};

// color enum from cam
class Detector
{
private:
    int letzer_index = 0;

public:
    Detector();
    ~Detector();
    int detect_line_direction(Camera pcam); // input colored_image in this function
    side detect_green_dot();
};
