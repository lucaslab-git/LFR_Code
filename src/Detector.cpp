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

    /*// Print in Serial Monitor
    for (int i = 0; i < 80; i++)
    {
        if (rectangle_line_array[i] == black)
        {
            Serial.print("\e[40m   "); // 40 = black
        }
        else if (rectangle_line_array[i] == white)
        {
            Serial.print("\e[47m   "); // 40 = black
        }
        else
        {
            Serial.print("\e[0m   ");
        }
    }
    Serial.println("\e[0m");*/

    // gruop areas
    area areas[80];
    int count_area = 0;
    for (int i = 0; i < 80; i++)
    {
        if (rectangle_line_array[i] == black)
        {

            areas[count_area].start_winkel = i * 4.5;
            // Serial.print("\e[44m" + String(areas[count_area].start_winkel)); // 40 = black
            for (int j = 0; j < 80; j++)
            {

                if (rectangle_line_array[i + j] != black && rectangle_line_array[i + j + 1] != black)
                {
                    areas[count_area].end_winkel = (i + j) * 4.5;
                    // Serial.print("\e[43m" + String(areas[count_area].end_winkel));
                    i = i + j;
                    count_area++;

                    break;
                }
                if (j > 0)
                {
                    // Serial.print("\e[45m   ");
                }
            }
        }
        else if (rectangle_line_array[i] == white)
        {
            // Serial.print("\e[47m   ");
        }
        else
        {
            // Serial.print("\e[0m   ");
        }
    }

    // Serial.println("\e[0m");

    for (int i = 0; i < count_area; i++)
    {

        areas[i].median_winkel = (areas[i].start_winkel + areas[i].end_winkel) / 2;
        // Serial.println(String(areas[i].median_winkel));
    }

    int minDifference = 360; // Initialisiere mit maximal möglichem Unterschied
    int winkel;

    for (int i = 0; i < count_area; i++)
    {
        // Berechne den Unterschied zwischen dem Zielwinkel und dem aktuellen Winkel
        int difference = abs(letzer_winkel - areas[i].median_winkel);

        // Berücksichtige den Übergang von 350 zu 0 Grad
        if (areas[i].median_winkel < letzer_winkel)
        {
            difference = min(difference, abs(letzer_winkel - (areas[i].median_winkel + 360)));
        }
        else
        {
            difference = min(difference, abs(letzer_winkel - (areas[i].median_winkel - 360)));
        }

        // Aktualisiere den am nächsten liegenden Winkel, falls notwendig
        if (difference < minDifference)
        {
            minDifference = difference;
            winkel = areas[i].median_winkel;
        }
    }

    letzer_winkel = winkel;
    Serial.print("Nähester Winkel: ");
    Serial.println(int(winkel));
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