#pragma once

#include <Arduino.h>
#include "LCD.h"
#include "Timer.h"

class OutputModule
{
public:
    OutputModule(LCD* lcdScreen);

    void processStart();
    void processStop();
    void processReset();
    void processSetTime(int minutes, int seconds);
    void processTick();
private:

    void enableLed();
    void disableLed();
    void switchToMainTimer(bool reset = true);
    void updateLcdTime(Timer *timer);
    void processElapsed();

    bool isOnMainTimer;
    Timer *mainTimer;
    Timer *secondaryTimer;
    LCD* lcd;
};