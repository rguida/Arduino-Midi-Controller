#pragma once

#include <stdint.h>

enum class Setting : uint8_t
{
    FunctionMode = 0,
    NumOfProgramsPerBank
};

class Settings
{
public:
    static constexpr uint8_t UndefinedValue{0xff};

    Settings();
    uint8_t GetValue(Setting setting);
    void SetValue(Setting setting, uint8_t value);
    void Save();

private:
    static constexpr uint8_t numOfSettings{2};
    uint8_t m_settingValues[numOfSettings];
};
