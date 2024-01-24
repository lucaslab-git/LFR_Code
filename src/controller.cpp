#include <Arduino.h>
#include "Motor.h"
#include "Camera.h"
// put function declarations here:

Motor m1(0, 12, 13);
Motor m2(120, 15, 14);
Motor m3(240, 2, 16);
Camera cam;

void setup()
{
  Serial.begin(115200);
  Serial.println("Serial init");
  cam.init_camera();
  cam.make_picture();
  cam.downscale();
  cam.zoom_in();
  cam.convert_to_color();
  delay(5000);

  /*
    m1.drive_direction(10);
    m2.drive_direction(10);
    m3.drive_direction(10);
    delay(1000);
    m1.stop();
    m2.stop();
    m3.stop();
  */
}

void loop()
{
  // cam.make_picture();

  delay(100);
}
