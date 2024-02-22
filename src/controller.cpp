#include <Arduino.h>
#include "Detector.h"
#include "Camera.h"
#include "FastLED.h"

Camera cam;
Detector detector;

#define DATA_PIN 15
#define NUM_LEDS 4

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
  led[0] = CRGB(150, 150, 150);
  led[1] = CRGB(150, 150, 150);
  led[2] = CRGB(150, 150, 150);
  led[3] = CRGB(150, 150, 150);

  FastLED.show();

  delay(500);

  cam.init_camera();
  cam.make_picture();
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
  // Serial.println("drive_direction " + dir);
  while (!Serial2.available())
  {
  }
  String recived_text = Serial2.readString();
  Serial.println(recived_text);
}