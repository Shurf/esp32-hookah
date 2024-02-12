#pragma once

#include <Arduino.h>
#include <XPT2046_Touchscreen.h>
#include "common.h"

#define TFT_CS 5

#define PHYSICAL_X_MIN 480
#define PHYSICAL_X_MAX 3910

#define PHYSICAL_Y_MIN 200
#define PHYSICAL_Y_MAX 3925

class TouchScreen {
public:
    TouchScreen();
    bool isTouched();
    void getTouchPoint(lv_point_t& p, touch_rot_t rot);
    bool getWasTouched();
    void setWasTouched(bool value);
private:
    uint16_t remap(uint16_t value, uint16_t  old_min, uint16_t  old_max, uint16_t  new_min, uint16_t new_max);
    XPT2046_Touchscreen* tft;
    bool wasTouched;
};
