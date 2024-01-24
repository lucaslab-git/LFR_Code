#pragma once

enum color
{
    white,
    black,
    green,
    red,
    grey
};

class Camera
{
private:
    /* data */
public:
    Camera();
    ~Camera();
    void init_camera();
    void make_picture();
    void downscale();
    void zoom_in();
    void convert_to_color();
    color get_colored_image(int index);
};
