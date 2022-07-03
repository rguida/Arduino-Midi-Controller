#pragma once

#include <inttypes.h>
#include <SoftwareSerial.h>

class MidiBus
{
public:
    MidiBus(uint8_t txPin, uint8_t rxPin);
    uint8_t GetChannel() const;
    void SetChannel(uint8_t channel);
    void SendProgramChange(uint8_t program);
    void SendControlChange(uint8_t control, uint8_t value);

private:
    static constexpr uint16_t MidiBusSpeed{31250};

    uint8_t m_channel{0};
    SoftwareSerial m_midiSerial;
};
