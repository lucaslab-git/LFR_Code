#include <Arduino.h>
#include "Motor.h"
#include "Camera.h"

Motor m1(0, 12, 13);
Motor m2(120, 15, 14);
Motor m3(240, 2, 16);
Camera cam;

void setup()
{
  Serial.begin(115200);

  Serial.println("Serial init");

  m1.stop();
  m2.stop();
  m3.stop();
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

  m1.stop();
  m2.stop();
  m3.stop();
}

void loop()
{
}