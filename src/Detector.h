enum side
{
    left,
    right
};
class Detector
{
private:
    /* data */
public:
    Detector(/* args */);
    ~Detector();
    int detect_line_direction(); // input colored_image in this function
    side detect_green_dot();
};