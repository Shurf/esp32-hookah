#include "HttpControl.h"

void HttpControl::sendRequest(bool primary)
{
    JsonDocument doc;
    String requestBody;
    HTTPClient http;

    Serial.println("sending");
    
    doc["led_profile_name"] = PROFILE_NAME;
    doc["primary"] = primary;

    serializeJson(doc, requestBody);

    http.begin(setPrimaryUrl);
    http.addHeader("Content-Type", "application/json");
    http.POST(requestBody);
}

void HttpControl::setPrimary()
{
    sendRequest(true);
}

void HttpControl::setSecondary()
{
    sendRequest(false);
}