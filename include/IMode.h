#pragma once

class IMode
{
public:
    virtual ~IMode() = default;
    virtual void Update() = 0;
    virtual void SetSwitch1(bool) = 0;
    virtual void SetSwitch2(bool) = 0;
    virtual void Increment() = 0;
    virtual void Decrement() = 0;
    virtual void Select() = 0;
};
