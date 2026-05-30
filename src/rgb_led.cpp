#include <Arduino.h>

#include "common.h"
#include "rgb_led.h"
#include "colors.h"

RGBLed::RGBLed(int r_port, int g_port, int b_port) {
    _r_port = r_port;
    _g_port = g_port;
    _b_port = b_port;

    Serial.println((String)"Init RGB LED on ports r:" + _r_port + ", g:" + _g_port + ", b:" + _b_port);
    pinMode(_r_port, OUTPUT);
    pinMode(_g_port, OUTPUT);
    pinMode(_b_port, OUTPUT);

    setColor(RED);
    delay(100);
    setColor(GREEN);
    delay(100);
    setColor(BLUE);
    delay(100);
    setColor(OFF);
}

void RGBLed::setColor(int color) {
    analogWrite(_r_port, (color >> 16) & 0xFF); // Extract red component
    analogWrite(_g_port, (color >> 8) & 0xFF); // Extract green component
    analogWrite(_b_port, color & 0xFF); // Extract blue component
}