#include "TouchScreen.h"

TouchScreen::TouchScreen() {
    tft = new XPT2046_Touchscreen(TFT_CS);
    tft->begin();
    wasTouched = false;
}

bool TouchScreen::isTouched()
{
    return tft->touched();
}

void TouchScreen::getTouchPoint(lv_point_t& p, touch_rot_t rot)
{
    TS_Point physicalPoint = tft->getPoint();
    
    switch (rot) {
        case 0:    p.x = (lv_coord_t) (MAX_WIDTH - remap(physicalPoint.y, PHYSICAL_Y_MIN, PHYSICAL_Y_MAX, 0, MAX_WIDTH));
                   p.y = (lv_coord_t) (MAX_HEIGHT - remap(physicalPoint.x, PHYSICAL_X_MIN, PHYSICAL_X_MAX, 0, MAX_HEIGHT));
                   break;
        // TODO: calculate
        case 1:    p.x = (lv_coord_t) remap(physicalPoint.y, 0, MAX_HEIGHT, 0, MAX_HEIGHT);
                   p.y = (lv_coord_t) remap(physicalPoint.x, 0, MAX_WIDTH, 0, MAX_WIDTH);
                   break;
        case 2:    p.x = (lv_coord_t) remap(physicalPoint.x, 0, MAX_WIDTH, MAX_WIDTH, 0);
                   p.y = (lv_coord_t) remap(physicalPoint.y, 0, MAX_HEIGHT, MAX_HEIGHT, 0);
                   break;
        case 3:    p.x = (lv_coord_t) remap(physicalPoint.y, 0, MAX_HEIGHT, MAX_HEIGHT, 0);
                   p.y = (lv_coord_t) remap(physicalPoint.x, 0, MAX_WIDTH, MAX_WIDTH, 0);
                   break;
    }
}

uint16_t TouchScreen::remap(uint16_t value, uint16_t  old_min, uint16_t  old_max, uint16_t  new_min, uint16_t new_max)
{
    return (((value - old_min) * (new_max - new_min)) / (old_max - old_min)) + new_min;
}

bool TouchScreen::getWasTouched()
{
    return wasTouched;
}

void TouchScreen::setWasTouched(bool value)
{
    wasTouched = value;
}