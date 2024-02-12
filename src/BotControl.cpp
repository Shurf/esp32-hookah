#include "BotControl.h"

Bot::Bot(WiFiClientSecure &client)
{

    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
    bot = new UniversalTelegramBot(BOTtoken, client);
    lastTimeBotRan = millis();
    clearRequests();
}

void Bot::clearRequests()
{
    startRequested = false;
    stopRequested = false;
    resetRequested = false;
    setTimeRequested = false;
}

void Bot::checkNewMessages()
{
    if (millis() > lastTimeBotRan + botRequestDelay)
    {
        int numNewMessages = bot->getUpdates(bot->last_message_received + 1);

        while (numNewMessages)
        {            
            handleNewMessages(numNewMessages);            
            numNewMessages = bot->getUpdates(bot->last_message_received + 1);
        }
        lastTimeBotRan = millis();
    }
}

int Bot::splitBySpaces(String text, String *stringArray, int stringArraySize)
{
    int i = 0;
    int k = 0;
    int n = 0;
    int cnt = 0;
    char string[MAX_STRING_LENGTH];

    while (cnt < stringArraySize)
    {
        while (text[i] != ' ' && text[i])
        {
            string[k] = text[i];
            i++;
            k++;
        }
        i++;
        string[k] = 0;
        stringArray[n] = string;
        k = 0;
        n++;
        cnt++;
    }

    return n;
}

void Bot::processSetTimeCommand(String text)
{
    String stringArray[2];
    char output[MAX_STRING_LENGTH];

    if(splitBySpaces(text, stringArray, 2) != 2)
        return;

    sscanf(stringArray[1].c_str(), "%d:%d", &requestedMinutes, &requestedSeconds);
    setTimeRequested = true;
}

void Bot::processResetCommand()
{
    resetRequested = true;
}

void Bot::processStartCommand()
{
    startRequested = true;
}

void Bot::processStopCommand()
{
    stopRequested = true;
}


void Bot::botSetup()
{
    /*bool result = bot->setMyCommands(F("["
                            "{\"command\":\"" RESET_TO_START "\", \"description\":\"Reset to night start\"},"
                            "{\"command\":\"" SET_TIME_AND_SESSION "\", \"description\":\"Set time and session\"},"
                            "{\"command\":\"" SET_TIME "\", \"description\":\"Set time\"},"
                            "{\"command\":\"" ADD_TIME "\", \"description\":\"Add time\"},"
                            "{\"command\":\"" UPDATE_FROM_SERVER "\", \"description\":\"Update from server\"}"
                            "]"
    ));*/
}

void Bot::sendBotControlMessage(String &chat_id)
{
    String message;
    String currentTime;
    message = "Hello";

    bot->sendMessage(chat_id, message, "");
    String keyboardJson = F(
        "["
        "[{\"text\":\"Reset\", \"callback_data\":\"" RESET_REQUEST "\"}, {\"text\":\"Start\", \"callback_data\":\"" START_REQUEST "\"}," 
        "{\"text\":\"Stop\", \"callback_data\":\"" STOP_REQUEST "\"}]," 
        "[{\"text\":\"5 min\", \"callback_data\":\"" SET_TIME_REQUEST " 5:00\"}," 
        "{\"text\":\"7 min\", \"callback_data\":\"" SET_TIME_REQUEST " 7:00\"},"
        "{\"text\":\"10 min\", \"callback_data\":\"" SET_TIME_REQUEST " 10:00\"}]"
        "]");
    bot->sendMessageWithInlineKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson);
}

// Handle what happens when you receive new messages
void Bot::handleNewMessages(int numNewMessages)
{


    for (int i = 0; i < numNewMessages; i++)
    {
        String chat_id = String(bot->messages[i].chat_id);
        if (chat_id != CHAT_ID)
        {
            bot->sendMessage(chat_id, "Unauthorized user", "");
            continue;
        }

        String text = bot->messages[i].text;

        String from_name = bot->messages[i].from_name;

        if (text == START)
        {
            String welcome = "Welcome, " + from_name + ".\n";
            welcome += "Use the following commands to control your outputs.\n\n";
            welcome += "" RESET_REQUEST " - reset\n";
            welcome += "" SET_TIME_REQUEST " <time> - set cycle time\n";
            welcome += "" START_REQUEST " - start the timer\n";
            welcome += "" STOP_REQUEST " - stop the timer\n";
            bot->sendMessage(chat_id, welcome, "");
            sendBotControlMessage(chat_id);
        }

        if (text.startsWith(RESET_REQUEST))
            processResetCommand();            
        else if (text.startsWith(SET_TIME_REQUEST))
            processSetTimeCommand(text);
        else if (text.startsWith(START_REQUEST))
            processStartCommand();
        else if (text.startsWith(STOP_REQUEST))
            processStopCommand();        
        sendBotControlMessage(chat_id);
    }
}