#include "MidiBus.h"
#include "DebugHelpers.h"

MidiBus::MidiBus(uint8_t txPin, uint8_t rxPin)
    : m_midiSerial{rxPin, txPin}
{
    m_midiSerial.begin(MidiBusSpeed);
}

uint8_t MidiBus::GetChannel() const
{
    return m_channel;
}

void MidiBus::SetChannel(uint8_t channel)
{
    m_channel = channel;
}

void MidiBus::SendProgramChange(uint8_t program)
{
    mylog(F("MidiBus::SendProgramChange %d"), program);

    if (m_channel > 0x0f)
        return;

    uint8_t command = 0xC0 + m_channel;

    m_midiSerial.write(command);
    m_midiSerial.write(program);
    m_midiSerial.flush();
}

void MidiBus::SendControlChange(uint8_t control, uint8_t value)
{
    if (m_channel > 0x0f)
        return;

    uint8_t command = 0xB0 + m_channel;

    m_midiSerial.write(command);
    m_midiSerial.write(control);
    m_midiSerial.write(value);
    m_midiSerial.flush();
}