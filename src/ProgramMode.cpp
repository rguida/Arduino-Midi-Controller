#include "ProgramMode.h"
#include <EEPROM.h>

ProgramMode::ProgramMode(Adafruit_SSD1306 &display)
    : m_display{display}
{
    for (auto i = 0; i < numOfSettings; ++i)
    {
        auto eepromValue = EEPROM.read(i);

        if (eepromValue != 0xff)
        {
            m_settingValues[i] = eepromValue;
        }
    }
}

void ProgramMode::Increment()
{
    UpdateMenuItem(++m_currentMenuItem);
}

void ProgramMode::Decrement()
{
    UpdateMenuItem(--m_currentMenuItem);
}

void ProgramMode::UpdateMenuItem(uint8_t newItem)
{
    for (auto i = 0; i < numOfMenuItems; ++i)
    {
        if (m_menuItems[i].parentId == m_currentParentId)
        {
            if (m_menuItems[i].id == newItem)
            {
                m_currentMenuItem = newItem;
                m_isChanged = true;
            }
        }
    }
}

void ProgramMode::Select()
{
    m_settingValues[m_currentParentId] = m_currentMenuItem & 0x0f;
}

void ProgramMode::Update()
{
    if (!m_isChanged)
    {
        return;
    }

    m_display.clearDisplay();
    m_display.setTextColor(SSD1306_WHITE);
    m_display.setTextSize(2);

    if (m_currentParentId != 0xff)
    {
        m_display.println(m_menuItems[m_currentParentId].text);
    }
    else
    {
        m_display.println(F("Main Menu"));
    }
    
    m_display.setTextSize(1);
    m_display.println(m_menuItems[m_currentMenuItem].text);

    uint8_t currentMenuItemValue = m_currentMenuItem & 0x0f;
    uint8_t settingIdx = m_currentParentId & 0xf0;

    if (currentMenuItemValue == m_settingValues[settingIdx])
    {
        m_display.drawCircle(m_display.width() - 10, m_display.height() - 10, 10, SSD1306_WHITE);
    }

    m_display.display();
}