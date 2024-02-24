#pragma once

enum color
{
    white,
    black,
    green,
    red,
    grey
};

struct pixel
{
    int red;
    int green;
    int blue;
};

class Camera
{
private:
public:
    color color_array[20][20];
    Camera();
    ~Camera();

    void init_camera();
    void make_picture();

    void downscale();
    void zoom_in();
    void convert_to_color();
};
