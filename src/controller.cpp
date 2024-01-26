#include <Arduino.h>
#include "Motor.h"
#include "Camera.h"

Motor m1(0, 12, 13, 0);
Motor m2(120, 14, 15, 0);
Motor m3(240, 2, 16, 0);
Camera cam;

void setup()
{
  Serial.begin(115200);

  Serial.println("Serial init");

  m1.stop();
  m2.stop();
  m3.stop();

  delay(1000);
  /*
    cam.init_camera();
    cam.make_picture();
    cam.downscale();
    cam.zoom_in();
    cam.convert_to_color();

    delay(2000);
  */
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

  m1.stop();
  m2.stop();
  m3.stop();
}

void loop()
{
}