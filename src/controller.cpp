#include <Arduino.h>
#include "Motor.h"
#include "Camera.h"
// put function declarations here:

Motor m1(3);
Camera cam;
int myFunction(int, int);

void setup()
{

  int result = myFunction(2, 3);
  Serial.begin(115200);
  Serial.printf("Before init failed with error 0x%x");
  Serial.println(result);
  cam.init_camera();
  cam.make_picture();
  cam.downscale();
  cam.zoom_in();
  cam.convert_to_color();
  delay(5000);
}

void loop()
{
  // cam.make_picture();

  delay(100);
}

// put function definitions here:
int myFunction(int x, int y)
{
  return x + y;
}