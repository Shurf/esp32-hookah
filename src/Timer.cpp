#include "Timer.h"

Timer::Timer(int minutes, int seconds, char direction)
{
    initialMinutes = minutes;
    initialSeconds = seconds;

    currentMinutes = minutes;
    currentSeconds = seconds;

    timerDirection = direction;

    isRunning = false;
    startMillis = 0;

    timeChanged = false;
    elapsed = false;
}

void Timer::setRunning(bool value)
{
    if(!isRunning && value)
        startMillis = millis();
    isRunning = value;
}

void Timer::decreaseBySecond()
{
    if(currentSeconds > 0)
    {
        currentSeconds--;
        return;
    }
    if(currentMinutes == 0)
    {
        elapsed = true;
        return;
    }
    currentSeconds = SECONDS_IN_MINUTE - 1;
    currentMinutes--;
}

void Timer::increaseBySecond()
{
    if(currentSeconds < SECONDS_IN_MINUTE)
    {
        currentSeconds++;
        return;
    }
    currentSeconds = 0;
    currentMinutes++;
}

void Timer::updateTimer()
{

    if(!isRunning)
        return;

    unsigned long currentMillis = millis();

    //TODO: use RTC
    int passedSeconds = (currentMillis - startMillis) / MILLIS_IN_SECOND;
    if(passedSeconds <= 0)
        return;

    startMillis = currentMillis;
    timeChanged = true;
    for(long i = 0; i < passedSeconds; i++)
    {
        if(timerDirection == DOWN)
            decreaseBySecond();
        else
            increaseBySecond();
    }
}

void Timer::clearTimeChanged()
{
    timeChanged = false;
}

bool Timer::isTimeChanged()
{
    return timeChanged;
}

void Timer::clearElapsed()
{
    elapsed = false;
}

bool Timer::isElapsed()
{
    return elapsed;
}

void Timer::reset()
{
    currentMinutes = initialMinutes;
    currentSeconds = initialSeconds;
}

int Timer::getCurrentMinutes()
{
    return currentMinutes;
}

int Timer::getCurrentSeconds()
{
    return currentSeconds;
}

void Timer::setTime(int minutes, int seconds)
{
    initialMinutes = minutes;
    initialSeconds = seconds;
    reset();
}