#ifndef RGBLED_H
#define RGBLED_H

// The RGBLed class controls an RGB LED with three separate pins
class RGBLed {
    public:
        RGBLed(int, int, int);
        void setColor(int color);
    private:
        int _r_port;
        int _g_port;
        int _b_port;
};

#endif