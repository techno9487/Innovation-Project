/**
 * Author: Toby Dunn
 * Purpose: Base class for any IoT device in our smart home network
 */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define RELAY_PIN 0

//Buffer for all packets being received?
char packetBuffer[1024];

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  
}

