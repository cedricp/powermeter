#include "PZEM004T.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Connect to the WiFi
const char* ssid = "Dear John";
const char* password = "password123";
const char* mqtt_server = "macman";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
const byte ledPin = 0; // Pin with LED on Adafruit Huzzah
PZEM004T pzem(&Serial); // Connect to PZEM via HW_serial
 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0')
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(ledPin, HIGH);
  if (receivedChar == '1')
   digitalWrite(ledPin, LOW);
  }
  Serial.println();
}
 
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
   // Attempt to connect
   if (client.connect("ESP8266 Client")) {
    client.subscribe("ledStatus");
   } else {
    delay(5000);
    }
 }
}
 
void setup()
{
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 
 pinMode(ledPin, OUTPUT);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
