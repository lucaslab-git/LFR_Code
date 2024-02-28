#include "Camera.h"
enum side
{
    left,
    right,
    up,
    down,
    none
};

struct area
{
    int start_winkel;
    int median_winkel;
    int end_winkel;
};

// color enum from cam
class Detector
{
private:
public:
    int letzer_winkel = 0;
    Detector();
    ~Detector();
    int detect_line_direction(Camera pcam); // input colored_image in this function
    int detect_green_dot(Camera pcam);
};
