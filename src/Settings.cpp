#include "Settings.h"
#include <EEPROM.h>

Settings::Settings()
{
    for (auto i = 0; i < numOfSettings; ++i)
    {
        auto eepromValue = EEPROM.read(i);

        if (eepromValue != UndefinedValue)
        {
            m_settingValues[i] = eepromValue;
        }
    }
}

uint8_t Settings::GetValue(Setting setting)
{
    return m_settingValues[static_cast<uint8_t>(setting)];
}

void Settings::SetValue(Setting setting, uint8_t value)
{
    m_settingValues[static_cast<uint8_t>(setting)] = value;
}

void Settings::Save()
{
    for (auto i = 0; i < numOfSettings; ++i)
    {
        EEPROM.update(i, m_settingValues[i]);
    }   
}