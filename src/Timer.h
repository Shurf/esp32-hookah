#include <Arduino.h>

#include "common.h"

#define DOWN 0
#define UP 1

class Timer {
public:
    Timer(int minutes = DEFAULT_MINUTES, int seconds = DEFAULT_SECONDS, char direction = DOWN);
    void updateTimer();
    void setRunning(bool value);
    bool timerRunning(){return isRunning;}
    bool isAtStart(){return currentMinutes == initialMinutes && currentSeconds == initialSeconds;}

    void clearTimeChanged();
    bool isTimeChanged();

    void clearElapsed();
    bool isElapsed();
    void reset();

    int getCurrentMinutes();
    int getCurrentSeconds();

    void setTime(int minutes, int seconds);

private:

    void decreaseBySecond();
    void increaseBySecond();

    int currentMinutes;
    int currentSeconds;

    int initialMinutes;
    int initialSeconds;

    bool isRunning;
    bool elapsed;
    bool timeChanged;
    unsigned long startMillis;
    char timerDirection;
};