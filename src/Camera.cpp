#include "Camera.h"
#include "esp_camera.h"
#include "Arduino.h"
#include "soc/soc.h"          // Disable brownour problems
#include "soc/rtc_cntl_reg.h" // Disable brownour problems
#include "driver/rtc_io.h"
#include <EEPROM.h>

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
}

uint8_t *rgb_image = new uint8_t[57600]; // set limit 57600

void Camera::make_picture()
{
    camera_fb_t *fb = NULL;

    fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Camera capture failed");
        return;
    }
    else
    {
        fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, rgb_image);
        Serial.println("Camera captured and stored");
    }

    esp_camera_fb_return(fb);
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
            Serial.print(avgR);
            Serial.print(" ");
            Serial.print(avgG);
            Serial.print(" ");
            Serial.print(avgB);
            Serial.print(" ");
        }
    }
    Serial.println("\e[1;31m downscaled sucessfully \e[1;37m");
};

uint8_t *zoomed_in = new uint8_t[1200]; // set limit 2304

void Camera::zoom_in()
{
    int zoomedInIndex = 0;
    int lineoffset = 0;
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
            zoomed_in[zoomedInIndex] = red;
            zoomed_in[zoomedInIndex + 1] = green;
            zoomed_in[zoomedInIndex + 2] = blue;
            zoomedInIndex = zoomedInIndex + 3;
            originalIndex = originalIndex + 3;
        }
        Serial.println("\e[0m");
        lineoffset = lineoffset + 36;
        originalIndex = originalIndex + 36;
    }

    // Serial.print("zoomed in");
};

uint8_t *color_output = new uint8_t[1200]; // set limit 2304
color *color_array = new color[400];       // set limit 2304

void Camera::convert_to_color()
{
    int index = 0;
    int color_array_index = 0;
    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
        {
            if (zoomed_in[index] < 100 && zoomed_in[index + 1] < 100 && zoomed_in[index + 2] < 100)
            {
                Serial.print("\e[40m   ");
                /*
                Serial.print(0);
                Serial.print(" ");
                Serial.print(0);
                Serial.print(" ");
                Serial.print(0);
                Serial.print(" ");
                */
                color_output[index] = 0;
                color_output[index + 1] = 0;
                color_output[index + 2] = 0;
                color_array[color_array_index++] = white;
            }
            else if (zoomed_in[index] < 100 && zoomed_in[index + 1] > 120 && zoomed_in[index + 2] < 100)
            {
                Serial.print("\e[42m   ");
                /*
                Serial.print(0);
                Serial.print(" ");
                Serial.print(255);
                Serial.print(" ");
                Serial.print(0);
                Serial.print(" ");
                */
                color_output[index] = 0;
                color_output[index + 1] = 255;
                color_output[index + 2] = 0;
                color_array[color_array_index++] = green;
            }
            else
            {
                Serial.print("\e[47m   ");
                /*
                Serial.print(255);
                Serial.print(" ");
                Serial.print(255);
                Serial.print(" ");
                Serial.print(255);
                Serial.print(" ");
                */
                color_output[index] = 255;
                color_output[index + 1] = 255;
                color_output[index + 2] = 255;
                color_array[color_array_index++] = black;
            }
            index = index + 3;
        };
        Serial.println("\e[0m");
    }
    Serial.println("converted color");
};

color Camera::get_colored_image(int index)
{
    return color_array[index];
};
