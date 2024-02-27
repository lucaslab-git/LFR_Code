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
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Serial2.setTimeout(20);
  Serial.println("Serial init");
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(led, NUM_LEDS);
  led[0] = CRGB(100, 100, 100);
  led[1] = CRGB(100, 100, 100);
  led[2] = CRGB(100, 100, 100);
  led[3] = CRGB(100, 100, 100);
  led[4] = CRGB(100, 100, 100);
  led[5] = CRGB(100, 100, 100);

  FastLED.show();

  delay(500);

  cam.init_camera();
  cam.make_picture();
  Serial2.println("drive_direction 0");
  delay(100000000);
  cam.downscale();
  cam.zoom_in();
  cam.convert_to_color();
  detector.detect_line_direction(cam);
}

void loop()
{
  cam.make_picture();
  cam.downscale();
  cam.zoom_in();
  cam.convert_to_color();
  String dir = String(detector.detect_line_direction(cam));
  Serial2.println("drive_direction " + dir);
  // Serial.println("drive dir" + dir);
  while (!Serial2.available())
  {
  }
  String recived_text = Serial2.readString();
  for (int i = 0; i < 9; i++)
  {
    if (recived_text.indexOf("A" + String(i)) > 0)
    {
      Serial.println("switch is pressed: " + String(i));
      delay(7000); // wait 7 sec for obstacle
    }
  }

  Serial.println(recived_text);
}