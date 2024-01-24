#include <Arduino.h>
#include "Motor.h"
#include "Camera.h"

Motor m1; // constructor not work!
Motor m2(120, 15, 14);
Motor m3(240, 2, 16);
Camera cam;

void setup()
{
  Serial.begin(115200);

  m1(0, 12, 13);
  Serial.println("Serial init");

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);

  m1.stop();
  m2.stop();
  m3.stop();

  /*
  digitalWrite(12, 0);
  digitalWrite(13, 0);
  digitalWrite(15, 0);
  digitalWrite(14, 0);
  digitalWrite(2, 0);
  digitalWrite(16, 0);
  */
  /*
    cam.init_camera();
    cam.make_picture();
    cam.downscale();
    cam.zoom_in();
    cam.convert_to_color();

    delay(2000);
  */
  m1.drive_direction(10);
  m2.drive_direction(10);
  m3.drive_direction(10);

  delay(1000);

  m1.stop();
  m2.stop();
  m3.stop();
}

void loop()
{
}