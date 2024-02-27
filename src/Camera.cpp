#include "Camera.h"
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>

// define the connection pins for the Camera used in init_camera()
#define EEPROM_SIZE 1
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::init_camera()
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QQVGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;

    // Init Camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
    sensor_t *s = esp_camera_sensor_get();

    s->set_wb_mode(s, 2);
}

// Array to save rgb image each pixel has 3 values rgb 160px * 120px * 3 values = 57600
uint8_t *rgb_image = new uint8_t[57600];

void Camera::make_picture()
{

    camera_fb_t *image = NULL;
    image = esp_camera_fb_get();

    // check if capture was sucessfull
    if (!image)
    {
        Serial.println("Camera capture failed");
        return;
    }
    else
    {
        // convert the (jpg) image to the rgb values
        fmt2rgb888(image->buf, image->len, PIXFORMAT_JPEG, rgb_image);
        // Serial.println("Camera captured and stored");
    }
    // return image becuase buffer overflow
    esp_camera_fb_return(image);
};

uint8_t *downscaled_image = new uint8_t[2304]; // set limit 2304

void Camera::downscale()
{
    memset(downscaled_image, 0, 2304);
    for (int y_out = 0; y_out < 24; ++y_out)
    {
        for (int x_out = 0; x_out < 32; ++x_out)
        {
            int x_start = x_out * (160 / 32);
            int y_start = y_out * (120 / 24);
            int x_end = (x_out + 1) * (160 / 32);
            int y_end = (y_out + 1) * (120 / 24);
            int sumR = 0, sumG = 0, sumB = 0;
            for (int y = y_start; y < y_end; ++y)
            {
                for (int x = x_start; x < x_end; ++x)
                {
                    int index = (y * 160 + x) * 3;
                    sumR += rgb_image[index];
                    sumG += rgb_image[index + 1];
                    sumB += rgb_image[index + 2];
                }
            }
            int avgR = sumR / ((x_end - x_start) * (y_end - y_start));
            int avgG = sumG / ((x_end - x_start) * (y_end - y_start));
            int avgB = sumB / ((x_end - x_start) * (y_end - y_start));

            int outIndex = (y_out * 32 + x_out) * 3;
            downscaled_image[outIndex] = static_cast<uint8_t>(avgR);
            downscaled_image[outIndex + 1] = static_cast<uint8_t>(avgG);
            downscaled_image[outIndex + 2] = static_cast<uint8_t>(avgB);
            /*Serial.print("\e[48;2;");
            Serial.print(avgR);
            Serial.print(";");
            Serial.print(avgG);
            Serial.print(";");
            Serial.print(avgB);
            Serial.print("m   ");*/
        }
        // Serial.println("\e[0m");
    }
    // Serial.println("\e[1;31m downscaled sucessfully \e[1;37m");
};

pixel zoomed_in[20][20];

void Camera::zoom_in()
{
    int originalIndex = 210;
    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
        {
            int red = downscaled_image[originalIndex];
            int green = downscaled_image[originalIndex + 1];
            int blue = downscaled_image[originalIndex + 2];

            Serial.print("\e[48;2;");
            Serial.print(red);
            Serial.print(";");
            Serial.print(green);
            Serial.print(";");
            Serial.print(blue);
            Serial.print("m   ");
            zoomed_in[x][y].red = red;
            zoomed_in[x][y].green = green;
            zoomed_in[x][y].blue = blue;
            originalIndex = originalIndex + 3;
        }
        Serial.println("\e[0m");
        originalIndex = originalIndex + 36;
    }

    Serial.println("\e[1;31m zoomed in sucessfully \e[1;37m");
};

// color color_array[24][24];                 // public attribut

void Camera::convert_to_color()
{
    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
        {
            if (zoomed_in[x][y].red > 50 && zoomed_in[x][y].green > 50 && zoomed_in[x][y].blue < 40)
            {
                Serial.print("\e[42m   "); // 42 = green
                color_array[x][y] = green;
            }
            else if (zoomed_in[x][y].red < 100 && zoomed_in[x][y].green < 100 && zoomed_in[x][y].blue < 100)
            {
                Serial.print("\e[40m   "); // 40 = black
                color_array[x][y] = black;
            }
            else
            {
                Serial.print("\e[47m   "); // 47 = white
                color_array[x][y] = white;
            }
        };
        Serial.println("\e[0m"); // 0 = transparent
    }
    Serial.println("converted color");
};
