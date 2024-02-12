#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "BotControl.h"
#include "TouchScreen.h"
#include "LCD.h"

class InputModule
{
public:
    InputModule(LCD* lcd);
    void handleInput();

    bool isResetRequested(){return resetRequested;};
    bool isStartRequested(){return startRequested;};
    bool isStopRequested(){return stopRequested;};
    bool isSetTimeRequested(){return setTimeRequested;};

    void clearRequests();
    void processRequests();

    void pollBot();


    int getRequestedMinutes(){return requestedMinutes;}
    int getRequestedSeconds(){return requestedSeconds;}
private:

    void processTouchScreen();
    void processPushButton();
    void processBot();

    bool isPushButtonPressed();

    WiFiClientSecure client;
    TouchScreen* touchScreen;
    Bot* bot;
    LCD* touchProcessor;        

    bool resetRequested;
    bool startRequested;
    bool stopRequested;
    bool setTimeRequested;

    int requestedMinutes;
    int requestedSeconds;
};