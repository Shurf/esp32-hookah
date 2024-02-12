#include "common.h"

void convertTimeToString(int minutes, int seconds, String& result)
{
    char tmp_str[20];
    sprintf(tmp_str, "%02d:%02d", minutes, seconds);
    result = tmp_str;
}