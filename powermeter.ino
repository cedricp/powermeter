#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "pubsubclient.h"
#include "PZEM004T.h"

// Connect to the WiFi
const char* ssid = "Dear John";
const char* password = "password123";
const char* mqtt_server = "minibian";

WiFiClient espClient;
PubSubClient client(espClient);
PZEM004T pzem(&Serial); // Connect to PZEM via HW_serial

unsigned long timer = millis();

void
callback(const char* topic, const byte* payload, const unsigned int length)
{
  String stopic = topic;
  String spayload = (const char*)payload;

}

void
reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(mqtt_server)) {
      //client.subscribe("powermeter");
    } else {
      delay(5000);
    }
  }
}

void
setup()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void
loop()
{
  if (!client.connected()) {
    reconnect();
  }

  if (timer - millis() > 2000){
    // Send report every 2 seconds
    String msg;
    msg += String(pzem.voltage()) + " V\n";
    msg += String(pzem.current()) + " A\n";
    msg += String(pzem.power()) + " W\n";
    msg += String(pzem.energy()) + " Wh";
    client.publish("powermeter/report", msg.c_str(), msg.length());
    timer = millis();
  }

  client.loop();
}
