#pragma once
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
    void get_colored_image();
};
