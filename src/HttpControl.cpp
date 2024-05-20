#include "HttpControl.h"

void HttpControl::sendRequest(JsonDocument& doc)
{
    String requestBody;
    HTTPClient http;
    
    doc["led_profile_name"] = PROFILE_NAME;
    

    serializeJson(doc, requestBody);

    http.begin(setPrimaryUrl);
    http.addHeader("Content-Type", "application/json");
    http.POST(requestBody);
}

void HttpControl::setPrimarySecondary(bool primary)
{
    JsonDocument doc;
    doc["primary"] = primary;
    sendRequest(doc);
}

void HttpControl::setPercentage(int percentage)
{
    JsonDocument doc;
    doc["percentage"] = percentage;
    sendRequest(doc);
}

void HttpControl::setPrimary()
{
    setPrimarySecondary(true);
}

void HttpControl::setSecondary()
{
    setPrimarySecondary(false);
}