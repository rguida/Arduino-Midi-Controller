#include "ProgramMode.h"
#include "DebugHelpers.h"

constexpr const MenuItem ProgramMode::m_menuItems[numOfMenuItems];

ProgramMode::ProgramMode(Adafruit_SSD1306 &display, Settings &settings)
    : m_display{display}, m_settings{settings}
{
}

void ProgramMode::Increment()
{
    UpdateMenuItem(m_menuItems[m_currentMenuItemIdx].id + 1);
}

void ProgramMode::Decrement()
{
    UpdateMenuItem(m_menuItems[m_currentMenuItemIdx].id - 1);
}

void ProgramMode::UpdateMenuItem(uint8_t newItemId)
{
    mylog(F("ProgramMode::UpdateMenuItem %d %d %d"), newItemId, m_currentMenuItemIdx, m_currentParentIdx);

    for (auto i = 0; i < numOfMenuItems; ++i)
    {
        if (m_menuItems[i].parentIndex == m_currentParentIdx)
        {
            if (newItemId == 0xfe)
            {
                if (m_menuItems[i].id != 0xff)
                {
                    // select the max id
                    m_currentMenuItemIdx = i;
                    m_currentParentIdx = m_menuItems[i].parentIndex;
                    m_isChanged = true;
                }
            }
            else
            {
                if ((m_menuItems[i].id == newItemId) || (m_menuItems[i].id == 0xff))
                {
                    m_currentMenuItemIdx = i;
                    m_currentParentIdx = m_menuItems[i].parentIndex;
                    m_isChanged = true;
                    mylog(F("ProgramMode::UpdateMenuItem %d %d"), m_currentMenuItemIdx, m_currentParentIdx);
                    break;
                }
            }
        }
    }
}

void ProgramMode::SetSwitch1(bool value)
{
    // NOP
}

void ProgramMode::SetSwitch2(bool value)
{
    // NOP
}

void ProgramMode::Select()
{
    mylog(F("ProgramMode::Select"));

    // exit sub menu
    if (m_menuItems[m_currentMenuItemIdx].id == 0xff)
    {
        if (m_currentMenuItemIdx == m_currentParentIdx)
        {
            return;
        }

        mylog(F("Exit sub menu"));
        m_currentMenuItemIdx = m_currentParentIdx;
        m_currentParentIdx = m_menuItems[m_currentMenuItemIdx].parentIndex;
        m_isChanged = true;
        return;
    }

    if (m_currentParentIdx != 0xff)
    {
        auto currentSetting = m_menuItems[m_currentParentIdx].id;
        auto currentValue = m_menuItems[m_currentMenuItemIdx].id;
        m_settings.SetValue(static_cast<Setting>(currentSetting), currentValue);
        m_settings.Save();
    }
    else
    {
        if (m_currentParentIdx == m_currentMenuItemIdx)
        {
            return;
        }

        m_currentParentIdx = m_currentMenuItemIdx;

        // select the first child
        for (auto i = 0; i < numOfMenuItems; ++i)
        {
            if (m_menuItems[i].parentIndex == m_currentParentIdx)
            {
                m_currentMenuItemIdx = i;
                break;
            }
        }
    }

    m_isChanged = true;
}

char *ProgramMode::GetMenuItemText(uint8_t menuItemIndex)
{
    mylog(F("menuText %d"), menuItemIndex);

    strcpy_P(m_lastMenuItemText, (char *)pgm_read_dword(&(string_table[menuItemIndex])));
    mylog(F("%s"), m_lastMenuItemText);
    return m_lastMenuItemText;
}

void ProgramMode::Update()
{
    if (!m_isChanged)
    {
        return;
    }

    mylog("ProgramMode::Update %d %d", m_currentParentIdx, m_currentMenuItemIdx);

    m_isChanged = false;

    m_display.clearDisplay();
    m_display.setCursor(0, 0);
    m_display.setTextColor(SSD1306_WHITE);
    m_display.setTextSize(2);

    if (m_currentParentIdx != Settings::UndefinedValue)
    {
        m_display.println(GetMenuItemText(m_currentParentIdx));
    }
    else
    {
        m_display.println(F("Main Menu"));
    }

    m_display.print(F(">"));
    m_display.println(GetMenuItemText(m_currentMenuItemIdx));

    if (m_currentParentIdx != 0xff)
    {
        uint8_t currentMenuItemValue = m_menuItems[m_currentMenuItemIdx].id;
        uint8_t currentSettingValue = m_settings.GetValue(static_cast<Setting>(m_currentParentIdx));
        mylog(F("%d %d"), currentMenuItemValue, currentSettingValue);

        if (currentMenuItemValue == currentSettingValue)
        {
            m_display.fillCircle(m_display.width() - 7, m_display.height() - 7, 5, SSD1306_WHITE);
        }
    }
    
    m_display.display();
}