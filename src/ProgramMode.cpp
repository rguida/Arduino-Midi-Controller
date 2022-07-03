#include "ProgramMode.h"
#include "DebugHelpers.h"
// {
//     {0x00, 0xff, "Func Mode"}
//   , {0x01, 0xff, "Live #Progs"}
//   , {0x11, 0x00, "Live"}
//   , {0x12, 0x00, "Looper"}
//   , {0x21, 0x01, "4 prgs/bank"}
//   , {0x22, 0x01, "6 prgs/bank"}
//   , {0x23, 0x01, "8 prgs/bank"}
// };

ProgramMode::ProgramMode(Adafruit_SSD1306 &display)
    : m_display{display}
{
    m_menuItems[0] = {0x00, 0xff};
    m_menuItems[1] = {0x01, 0xff};

    m_menuItems[2] = {0x00, 0};
    m_menuItems[3] = {0x01, 0};
    m_menuItems[4] = {0xff, 0};

    m_menuItems[5] = {0x00, 1};
    m_menuItems[6] = {0x01, 1};
    m_menuItems[7] = {0x02, 1};
    m_menuItems[8] = {0xff, 1};
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

char* ProgramMode::GetMenuItemText(uint8_t menuItemIndex)
{
    mylog(F("menuText %d"), menuItemIndex);

    strcpy_P(m_lastMenuItemText, (char*)pgm_read_dword(&(string_table[menuItemIndex])));
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

    uint8_t currentMenuItemValue = m_menuItems[m_currentMenuItemIdx].id;
    uint8_t currentSettingValue = m_settings.GetValue(static_cast<Setting>(m_currentParentIdx));
    mylog(F("%d %d"), currentMenuItemValue, currentSettingValue);
    
    if (currentMenuItemValue == currentSettingValue)
    {
        m_display.fillCircle(m_display.width() - 7, m_display.height() - 7, 5, SSD1306_WHITE);
    }

    m_display.display();
}