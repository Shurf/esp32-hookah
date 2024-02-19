#include "OutputModule.h"

OutputModule::OutputModule(LCD *lcdScreen)
{
    lcd = lcdScreen;
    mainTimer = new Timer();
    secondaryTimer = new Timer(0, 0, UP);

    switchToMainTimer();

    lcd->update(true);

    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    isOnMainTimer = true;
}

void OutputModule::enableLed()
{
    analogWrite(LED_RED_PIN, 255);
}

void OutputModule::disableLed()
{
    analogWrite(LED_RED_PIN, 0);
}

void OutputModule::switchToMainTimer(bool reset)
{
    disableLed();
    isOnMainTimer = true;
    if (reset)
        mainTimer->reset();
    secondaryTimer->reset();
    secondaryTimer->setRunning(false);
    lcd->setWhiteTimeColor();
    lcd->setNewTime(mainTimer->getCurrentMinutes(), mainTimer->getCurrentSeconds());
}

void OutputModule::updateLcdTime(Timer *timer)
{
    timer->updateTimer();
    if (timer->isTimeChanged())
    {
        lcd->setNewTime(timer->getCurrentMinutes(), timer->getCurrentSeconds());
        timer->clearTimeChanged();
    }
}

void OutputModule::processElapsed()
{
    Timer *newTimer = isOnMainTimer ? secondaryTimer : mainTimer;
    Timer *oldTimer = isOnMainTimer ? mainTimer : secondaryTimer;

    mainTimer->reset();
    secondaryTimer->reset();
    newTimer->setRunning(true);
    oldTimer->setRunning(false);
    if (isOnMainTimer)
    {
        lcd->setRedTimeColor();
        enableLed();
    }
    else
    {
        lcd->setWhiteTimeColor();
        disableLed();
    }
    lcd->setNewTime(newTimer->getCurrentMinutes(), newTimer->getCurrentSeconds());

    isOnMainTimer = !isOnMainTimer;
    oldTimer->clearElapsed();
}

void OutputModule::processStart()
{
    mainTimer->setRunning(true);
    lcd->processStartRequest();
}

void OutputModule::processStop()
{
    if (isOnMainTimer)
        mainTimer->setRunning(false);

    switchToMainTimer(false);
    lcd->processStopRequest();
}

void OutputModule::processReset()
{
    if(isOnMainTimer && !mainTimer->timerRunning() && mainTimer->isAtStart())
    {
        mainTimer->setRunning(true);
        return;
    }

    bool currentlyOnSecondaryTimer = !isOnMainTimer;
    switchToMainTimer(true);
    if (currentlyOnSecondaryTimer)
        mainTimer->setRunning(true);
}

void OutputModule::processSetTime(int minutes, int seconds)
{
    mainTimer->setTime(minutes, seconds);
    lcd->setNewTime(minutes, seconds);
    lcd->update(true);
}

void OutputModule::processTick()
{
    if (isOnMainTimer)
    {
        updateLcdTime(mainTimer);
        if (mainTimer->isElapsed())
            processElapsed();
    }
    else
    {
        updateLcdTime(secondaryTimer);
        if (secondaryTimer->isElapsed())
            processElapsed();
    }

    lcd->update();
}