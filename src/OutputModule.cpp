#include "OutputModule.h"

OutputModule::OutputModule(LCD *lcdScreen)
{
    lcd = lcdScreen;
    mainTimer = new Timer();
    secondaryTimer = new Timer(0, 0, UP);
    powerSavingTimer = new Timer(POWERSAVING_TIMEOUT_MINUTES, POWERSAVING_TIMEOUT_SECONDS);
    httpControl = new HttpControl();

    percentage = 100;

    switchToMainTimer();

    lcd->update(true);
    lcd->dimScreen();

    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_BLUE_PIN, OUTPUT);

    isOnMainTimer = true;
}

void OutputModule::enableLed()
{
    analogWrite(LED_RED_PIN, 255);
}

void OutputModule::setPrimaryIndication()
{
    lcd->setWhiteTimeColor();
    httpControl->setPrimary();
    disableLed();
}

void OutputModule::disableLed()
{
    analogWrite(LED_RED_PIN, 0);
}

void OutputModule::setSecondaryIndication()
{
    lcd->setRedTimeColor();
    httpControl->setSecondary();
    enableLed();
}

void OutputModule::switchToMainTimer(bool reset)
{
    setPrimaryIndication();
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
        auto current_seconds = timer->getCurrentMinutes()*SECONDS_IN_MINUTE + timer->getCurrentSeconds();
        auto max_seconds = timer->getInitialMinutes()*SECONDS_IN_MINUTE + timer->getInitialSeconds();
        httpControl->setPercentage(100*current_seconds / max_seconds);
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
        setSecondaryIndication();
    }
    else
    {        
        setPrimaryIndication();
    }
    lcd->setNewTime(newTimer->getCurrentMinutes(), newTimer->getCurrentSeconds());

    isOnMainTimer = !isOnMainTimer;
    oldTimer->clearElapsed();
}

void OutputModule::processStart()
{
    lcd->brightenScreen();
    mainTimer->setRunning(true);
    lcd->processStartRequest();
}

void OutputModule::processStop()
{
    if (isOnMainTimer)
        mainTimer->setRunning(false);

    switchToMainTimer(false);
    lcd->processStopRequest();
    powerSavingTimer->reset();
    powerSavingTimer->setRunning(true);
}

void OutputModule::processReset()
{
    if(isOnMainTimer && !mainTimer->timerRunning() && mainTimer->isAtStart())
    {
        processStart();
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
    powerSavingTimer->updateTimer();
    if(powerSavingTimer->isElapsed())
    {
        powerSavingTimer->setRunning(false);
        powerSavingTimer->reset();
        powerSavingTimer->clearElapsed();
        lcd->dimScreen();
        return;
    }
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