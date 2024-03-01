#include "Detector.h"
#include "Arduino.h"

Detector::Detector()
{
}

Detector::~Detector()
{
}

double Detector::detect_line_direction(Camera pcam)
{

    color rectangle_line_array[96];
    int anzahl_black = 0;
    for (int x = 0; x < 24; x++)
    {
        for (int y = 0; y < 24; y++)
        {
            if (pcam.color_array[x][y] == black)
            {
                anzahl_black++;
            }
        }
    }
    Serial.printf(" anzahl black %d ", anzahl_black);

    // load the rectangle counterwise from upperleft in the 1d array
    for (int i = 0; i < 24; i++)
    {
        rectangle_line_array[i] = pcam.color_array[i][0];
    }
    for (int i = 0; i < 24; i++)
    {
        rectangle_line_array[i + 24] = pcam.color_array[23][i];
    }
    for (int i = 0; i < 24; i++)
    {
        rectangle_line_array[i + 48] = pcam.color_array[23 - i][23];
    }
    for (int i = 0; i < 24; i++)
    {
        rectangle_line_array[i + 72] = pcam.color_array[0][23 - i];
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
    area areas[96];
    int count_area = 0;
    int modifier_count = 0;
    for (int i = 0; i < 96; i++)
    {
        if (rectangle_line_array[i] == black)
        {

            areas[count_area].start_winkel = i * 3.75;
            // Serial.print("\e[44m" + String(areas[count_area].start_winkel)); // 40 = black
            for (int j = 0; j < 96; j++)
            {

                if (areas[count_area].length > 10)
                {
                    modifier_count = 1;
                }
                areas[count_area].length++;
                if (rectangle_line_array[i + j] != black && rectangle_line_array[i + j + 1] != black)
                {
                    areas[count_area].end_winkel = (i + j) * 3.75;
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
            //  Serial.print("\e[47m   ");
        }
        else
        {
            // Serial.print("\e[0m   ");
        }
    }

    count_area = count_area + modifier_count;

    // Serial.println("\e[0m");

    for (int i = 0; i < count_area; i++)
    {

        areas[i].median_winkel = (areas[i].start_winkel + areas[i].end_winkel) / 2;
        // Serial.println(String(areas[i].median_winkel));
    }

    int minDifference = 360; // Initialisiere mit maximal möglichem Unterschied
    int winkel = 0;

    for (int i = 0; i < count_area; i++)
    {
        // Berechne den Unterschied zwischen dem Zielwinkel und dem aktuellen Winkel
        int difference = abs(letzer_winkel - areas[i].median_winkel);

        // Berücksichtige den Übergang von 350 zu 0 Grad
        if (areas[i].median_winkel < letzer_winkel)
        {
            difference = min(difference, abs(int(letzer_winkel) - (areas[i].median_winkel + 360)));
        }
        else
        {
            difference = min(difference, abs(int(letzer_winkel) - (areas[i].median_winkel - 360)));
        }

        if (difference < minDifference)
        {
            // Check if this calc is corrrect changed 28.02. 8.30
            minDifference = difference;
            if (areas[i].median_winkel < 60)
            {
                winkel = areas[i].median_winkel - 60 + 360; // 45 degree mounting offset
            }
            else
            {
                winkel = areas[i].median_winkel - 60; // 45 degree mounting offset
            }
        }
    }
    Serial.printf("area count %d ", count_area);

    int diff_angle;
    for (int i = 0; i < 360; i++)
    {
        if (letzer_winkel + i > 360)
        {
            if (letzer_winkel + i - 360 == winkel)
            {
                if (i < 140 || i > 220)
                {
                    letzer_winkel = winkel;
                    break;
                }
                else
                {
                    winkel = letzer_winkel;
                    break;
                }
            }
        }
        else
        {
            if (letzer_winkel + i == winkel)
            {
                if (i < 140 || i > 220)
                {
                    letzer_winkel = winkel;
                    break;
                }
                else
                {
                    winkel = letzer_winkel;
                    break;
                }
            }
        }
    }
    /*

   Serial.print("winkel Winkel: ");
   Serial.println(int(winkel));
    */

    return int(winkel);
};

int Detector::detect_green_dot(Camera pcam)
{
    color image[20][20];
    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            image[x][y] = pcam.color_array[x][y];
        }
    }
    for (int i = 0; i <= 20 - 3; i++)
    {
        for (int j = 0; j <= 20 - 3; j++)
        {

            bool isGreenArea = true;
            for (int x = i; x < i + 3; x++)
            {
                for (int y = j; y < j + 3; y++)
                {
                    if (image[x][y] != green)
                    {
                        isGreenArea = false;
                        break;
                    }
                }
                if (!isGreenArea)
                    break;
            }

            if (isGreenArea)
            {
                // Calculate the angle from the center to the center of the found area
                float centerX = i + 1.5;                                           // Center of the found area
                float centerY = j + 1.5;                                           // Center of the found area
                float centerX = i;                                                 // Center of the found area
                float centerY = j;                                                 // Center of the found area
                float angleRadians = atan2(10 - centerY, 10 - centerX) * 180 / PI; // Angle in radians
                return int(angleRadians);
            }
            else
            {
                return 361;
            }
        }
    }
}