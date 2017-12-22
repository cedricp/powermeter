#ifndef PZEM004T_H
#define PZEM004T_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <IPAddress.h>

struct PZEMCommand {
    uint8_t command;
    uint8_t addr[4];
    uint8_t data;
    uint8_t crc;
};

class PZEM004T
{
public:
    PZEM004T(HardwareSerial *port);
    ~PZEM004T();

    void setReadTimeout(unsigned long msec);
    unsigned long readTimeout() {return _readTimeOut;}

    float voltage();
    float current();
    float power();
    float energy();

    bool setAddress(const IPAddress &newAddr);
    bool setPowerAlarm(uint8_t threshold);

private:
    IPAddress ipaddr;
    Stream *serial;
    unsigned long _readTimeOut;

    void send(const IPAddress &addr, uint8_t cmd, uint8_t data = 0);
    bool recieve(uint8_t resp, uint8_t *data = 0);

    uint8_t crc(uint8_t *data, uint8_t sz);
};

#endif // PZEM004T_H
