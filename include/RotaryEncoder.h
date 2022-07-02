#pragma once

#include "ec11.hpp"
//#include <ArxSmartPtr.h>
#include <Bounce2.h>

enum class RotationState
{
    Clockwise = -1,
    Idle = 0,
    CounterClockwise = 1
};

class RotaryEncoder
{
public:
    RotaryEncoder();
    ~RotaryEncoder();
    RotaryEncoder(uint8_t clockPin, uint8_t dataPin, uint8_t switchPin);
    uint8_t GetClockPin() const;
    uint8_t GetDataPin() const;
    a21::EC11 &GetEncoder();
    bool GetSwitchState() const;
    RotationState GetRotationState() const;
    void Update();

private:
    static const uint8_t debounceDurationMs;

    a21::EC11 m_encoder;
    bool m_switchState;
    uint8_t m_clockPin;
    uint8_t m_dataPin;
    uint8_t m_switchPin;
    
    Bounce m_switchDebouncer;
    Bounce m_dataDebouncer;
    Bounce m_clockDebouncer;
    RotationState m_rotationState;

    static void readEncoder(void *param);
};