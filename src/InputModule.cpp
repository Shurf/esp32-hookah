#include "InputModule.h"

InputModule::InputModule(LCD* lcd)
{
    bot = new Bot(client);
    touchScreen = new TouchScreen();
    touchProcessor = lcd;

    pinMode(PUSHBUTTON_PIN, INPUT_PULLUP);

    clearRequests();
}

bool InputModule::isPushButtonPressed()
{
  return (digitalRead(PUSHBUTTON_PIN) == 0);
}

void InputModule::clearRequests()
{
    resetRequested = false;
    startRequested = false;
    stopRequested = false;
    setTimeRequested = false;
}

void InputModule::processPushButton()
{
    if(isPushButtonPressed())
        resetRequested = true;
}

void InputModule::processTouchScreen()
{
    if(!touchScreen->isTouched())
    {
        touchScreen->setWasTouched(false);
        return;
    }

    if(touchScreen->getWasTouched())
        return;
    touchScreen->setWasTouched(true);

    lv_point_t point;
    touchScreen->getTouchPoint(point, ROT_0);
    switch(touchProcessor->processTouch(point))
    {
        case START_REQUESTED:
            startRequested = true;
            break;
        case STOP_REQUESTED:
            stopRequested = true;
            break;
        case RESET_REQUESTED:
            resetRequested = true;
            break;
    }        
}

void InputModule::pollBot()
{
    bot->checkNewMessages();
}

void InputModule::processBot()
{
    
    if(bot->isStartRequested())
        startRequested = true;
    if(bot->isStopRequested())
        stopRequested = true;
    if(bot->isResetRequested())
        resetRequested = true;
    if(bot->isSetTimeRequested())
    {
        setTimeRequested = true;
        requestedMinutes = bot->getRequestedMinutes();
        requestedSeconds = bot->getRequestedSeconds();
    }
    bot->clearRequests();
}

void InputModule::processRequests()
{
    processPushButton();
    processTouchScreen();
    processBot();
}
