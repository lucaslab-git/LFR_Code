#include "Detector.h"

Detector::Detector(/* args */)
{
}

Detector::~Detector()
{
}

color *color_array[20][20]; // set limit 2304

int Detector::detect_line_direction(){

    // 2D array
    // am rand schauen wo (Oben unten, links rechts) die linie rein geht und raus geht bzw. wo sie raus geht und dann in
    // diese richtung fahren, vielleicht auch linie finden.
}; // input colored_image in this function

side Detector::detect_green_dot()
{
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (*color_array[x][y] == green)
            {
                int green_count = 0;
                for (int serch_y = 0; serch_y < 3; serch_y++)
                {
                    for (int serch_x = 0; serch_x < 3; serch_x++)
                    {
                        if (*color_array[x + serch_x][y + serch_y] == green)
                        {
                            green_count++;
                            if (green_count > 3)
                            {
                                return left;
                            }
                        }
                    }
                }
            }
        }
    };
    return none;
}