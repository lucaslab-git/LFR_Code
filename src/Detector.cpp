#include "Detector.h"
#include "Arduino.h"

Detector::Detector()
{
}

Detector::~Detector()
{
}

int Detector::detect_line_direction(Camera pcam)
{

    color rectangle_line_array[80];
    for (int i = 0; i < 80; i++)
    {
        rectangle_line_array[i] = white;
    }

    // load the rectangle counterwise from upperleft in the 1d array
    for (int i = 0; i < 20; i++)
    {
        rectangle_line_array[i] = pcam.color_array[i][0];
    }
    for (int i = 0; i < 20; i++)
    {
        rectangle_line_array[i + 20] = pcam.color_array[19][i];
    }
    for (int i = 0; i < 20; i++)
    {
        rectangle_line_array[i + 40] = pcam.color_array[19 - i][19];
    }
    for (int i = 0; i < 20; i++)
    {
        rectangle_line_array[i + 60] = pcam.color_array[0][19 - i];
    }

    for (int i = 0; i < 80; i++)
    {
        if (rectangle_line_array[i] == black)
        {
            // Serial.print("\e[40m   "); // 40 = black
        }
        else if (rectangle_line_array[i] == white)
        {
            // Serial.print("\e[47m   "); // 40 = black
        }
    }
    // Serial.print("\e[0m");

    int naehester_index;
    double winkel;
    for (int i = 0; i < 40; i++)
    {

        if ((letzer_index + i) > 79)
        {
            if (rectangle_line_array[letzer_index + i - 79] == black)
            {

                winkel = (letzer_index + i - 79) * 4.5;

                break;
            }
        }
        else
        {
            if (rectangle_line_array[letzer_index + i] == black)
            {

                winkel = (letzer_index + i) * 4.5;

                break;
            }
        }

        if ((letzer_index - i) < 0)
        {
            if (rectangle_line_array[letzer_index - i + 79] == black)
            {

                winkel = (letzer_index - i + 79) * 4.5;

                break;
            }
        }
        else
        {
            if (rectangle_line_array[letzer_index - i] == black)
            {

                winkel = (letzer_index - i) * 4.5;

                break;
            }
        }
    }

    // Serial.println("Nähester Winkel: " + String(nearest));
    // Serial.print(int(winkel));
    return int(winkel);
};

side Detector::detect_green_dot()
{
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            if (1)
            {
                int green_count = 0;
                for (int serch_y = 0; serch_y < 3; serch_y++)
                {
                    for (int serch_x = 0; serch_x < 3; serch_x++)
                    {
                        if (1)
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