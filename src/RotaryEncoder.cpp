#include "RotaryEncoder.h"
#include "DebugHelpers.h"
#include <ArduinoLog.h>

const uint8_t RotaryEncoder::debounceDurationMs = 50;

RotaryEncoder::RotaryEncoder()
{
    //.verboseln("RotaryEncoder::ctor");
}

RotaryEncoder::~RotaryEncoder()
{
    //mylog("RotaryEncoder::dtor");
}

RotaryEncoder::RotaryEncoder(uint8_t clockPin, uint8_t dataPin, uint8_t switchPin)
{
    //mylog("RotaryEncoder::ctor");

    m_clockPin = clockPin;
    m_dataPin = dataPin;
    m_switchPin = switchPin;

    m_switchDebouncer.attach(switchPin, INPUT_PULLUP);
    m_switchDebouncer.interval(debounceDurationMs * 2);

    m_dataDebouncer.attach(dataPin, INPUT_PULLUP);
    m_dataDebouncer.interval(debounceDurationMs);

    m_clockDebouncer.attach(clockPin, INPUT_PULLUP);
    m_clockDebouncer.interval(debounceDurationMs);
}

uint8_t RotaryEncoder::GetClockPin() const
{
    return m_clockPin;
}

uint8_t RotaryEncoder::GetDataPin() const
{
    return m_dataPin;
}

a21::EC11 &RotaryEncoder::GetEncoder()
{
    return m_encoder;
}

bool RotaryEncoder::GetSwitchState() const
{
    return m_switchState;
}

RotationState RotaryEncoder::GetRotationState() const
{
    return m_rotationState;
}

void RotaryEncoder::Update()
{
    for (int i = 0; i < 100; i++) {
        m_switchDebouncer.update();
        m_encoder.checkPins(digitalRead(m_dataPin), digitalRead(m_clockPin));
        //delay(1);
    }

    m_switchState = !m_switchDebouncer.read();
    //mylog("%d", m_switchState ? 1 : 0);

    a21::EC11Event e;

    if (m_encoder.read(&e))
    {
        if (e.type == a21::EC11Event::StepCW)
        {
            // Clock-wise.
            m_rotationState = RotationState::Clockwise;
            mylog(F("  cw"));
        }
        else
        {
            // Counter clock-wise.
            m_rotationState = RotationState::CounterClockwise;
            mylog(F("  ccw"));
        }
    }
    else
    {
        m_rotationState = RotationState::Idle;
        //mylog(F("  idle"));
    }
}

void RotaryEncoder::readEncoder(void *param)
{
    auto self = static_cast<RotaryEncoder *>(param);
    self->GetEncoder().checkPins(digitalRead(self->GetDataPin()), digitalRead(self->GetClockPin()));
}
