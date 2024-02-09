#include <Arduino.h>
// #include "Motor.h"
#include "Camera.h"
#include "FastLED.h"

// Motor m1(0, 12, 13, 0);
// Motor m2(120, 14, 15, 0);
// Motor m3(240, 2, 18, 0);
Camera cam;

#define DATA_PIN 15
#define NUM_LEDS 4

CRGB led[NUM_LEDS];

void setup()
{
  Serial.begin(115200);

  Serial.println("Serial init");
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(led, NUM_LEDS);
  led[0] = CRGB(40, 32, 40);
  led[1] = CRGB(40, 32, 40);
  led[2] = CRGB(40, 32, 40);
  led[3] = CRGB(40, 32, 40);

  FastLED.show();

  //  m1.stop();
  // m2.stop();
  // m3.stop();

  delay(1000);

  cam.init_camera();
  cam.make_picture();
  cam.downscale();
  cam.zoom_in();
  cam.convert_to_color();

  delay(2000);

  /*
   m1.drive_direction(0);
   m2.drive_direction(0);
   m3.drive_direction(0);

   delay(1000);

   m1.drive_direction(180);
   m2.drive_direction(180);
   m3.drive_direction(180);

   delay(1000);

   m1.drive_direction(270);
   m2.drive_direction(270);
   m3.drive_direction(270);

   delay(1000);

   m1.drive_direction(90);
   m2.drive_direction(90);
   m3.drive_direction(90);

   delay(1000);
   */
}

void loop()
{
}