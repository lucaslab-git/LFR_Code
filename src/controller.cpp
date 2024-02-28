#include <Arduino.h>
#include "Detector.h"
#include "Camera.h"
#include "FastLED.h"

Camera cam;
Detector detector;

#define DATA_PIN 15
#define NUM_LEDS 6

CRGB led[NUM_LEDS];

#define RXp2 13
#define TXp2 12

void setup()
{
  // Init Serial communicaiton
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial2.setTimeout(20);
  Serial.println("Welcome, start of program");

  // Turn on LED-ring for Camera
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(led, NUM_LEDS);
  led[0] = CRGB(40, 40, 40);
  led[1] = CRGB(40, 40, 40);
  led[2] = CRGB(40, 40, 40);
  led[3] = CRGB(40, 40, 40);
  led[4] = CRGB(40, 40, 40);
  led[5] = CRGB(40, 40, 40);
  FastLED.show();

  // wait so the awb can set correct
  delay(500);

  cam.init_camera();

  // wait until an button is touched and the Arduino send an command
  while (!Serial2.available())
  {
  }
  Serial.println(Serial2.readString());
}

int change_dir_green = 0;
void loop()
{
  // Camera macht ein Foto
  cam.make_picture();
  // Camera downscaled image auf 32px x 24px
  cam.downscale();
  // Camera zoomed in auf 20px x 20px
  cam.zoom_in();
  // Camera convert rgb image to enum color image
  cam.convert_to_color();
  // Detector calculate the direction of black line
  String dir = String(detector.detect_line_direction(cam));
  // Detector calculate the direction of green dot
  int green_dir = detector.detect_green_dot(cam);
  // If Green was detected (not 361) the robot drives dir_green
  if (green_dir != 361)
  {
    // Cooldown limit because green dot goes trought image
    if (change_dir_green <= 60) // Wert für grün anpassen
    {
      change_dir_green += 10;
      detector.letzer_winkel = green_dir;
    }
    Serial2.println("drive_direction " + String(detector.letzer_winkel));
  }
  else
  {
    // Cooldown from 60 cycles
    if (change_dir_green != 0)
    {
      change_dir_green--;
    }
    Serial2.println("drive_direction " + dir);
  }

  // wait for arduino to answer
  while (!Serial2.available())
  {
  }

  // Print out recived text
  String recived_text = Serial2.readString();
  Serial.println(recived_text);
  // Kann theoretisch weggelassen werden, da der ESP auf eine Antwort vom Esp wartet,
  // bis um hindernisgefahren vielleicht muss aber der letze winkel nue gesetzt werden
  // bei eintretem der Linie nach dem Hindernis.
  /*for (int i = 0; i < 9; i++)
  {
    if (recived_text.indexOf("A" + String(i)) > 0)
    {
      Serial.println("switch is pressed: " + String(i));
      delay(7000); // wait 7 sec for obstacle
    }
  }*/
}