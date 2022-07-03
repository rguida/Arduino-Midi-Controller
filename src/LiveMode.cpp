#include "LiveMode.h"
#include "DebugHelpers.h"

LiveMode::LiveMode(Adafruit_SSD1306 &display, Settings &settings)
    : m_display{display}
    , m_settings{settings}
{
    auto prgsPerBank = m_settings.GetValue(Setting::NumOfProgramsPerBank);

    m_bankSize = 4 + (prgsPerBank * 2);
    m_maxBanks = (MaxPrograms / m_bankSize) - 1;
}

void LiveMode::SetSwitch1(bool value)
{
    if (m_switch1 == value)
    {
        return;
    }

    m_switch1 = value;

    if (m_switch1)
    {
        if (m_lastProgram < m_bankSize - 1)
        {
            ++m_lastProgram;
        }
        else
        {
            m_lastProgram = 0;
        }

        m_isChanged = true;
    }
}

void LiveMode::SetSwitch2(bool value)
{
    if (m_switch2 == value)
    {
        return;
    }

    m_switch2 = value;

    if (m_switch2)
    {
        if (m_lastProgram > 0)
        {
            --m_lastProgram;
        }
        else
        {
            m_lastProgram = m_bankSize - 1;
        }

        m_isChanged = true;
    }
}

void LiveMode::Increment()
{
    if (m_lastBank < m_maxBanks)
    {
        ++m_lastBank;
    }
    else
    {
        m_lastBank = 0;
    }

    m_isChanged = true;
}

void LiveMode::Decrement()
{
    if (m_lastBank > 0)
    {
        --m_lastBank;
    }
    else
    {
        m_lastBank = m_maxBanks;
    }

    m_isChanged = true;
}

void LiveMode::Select()
{
    // NOP
}

void LiveMode::Update()
{
    if (!m_isChanged)
    {
        return;
    }

    mylog(F("LiveMode::Update %d %d %d"), m_bankSize, m_lastBank, m_lastProgram);
    m_isChanged = false;

    uint8_t absoluteProgram = (m_bankSize * m_lastBank) + m_lastProgram + 1;
    m_midiBus.SendProgramChange(absoluteProgram);

    DrawScreen();
}

void LiveMode::DrawScreen()
{
    m_display.clearDisplay();

    static constexpr uint8_t programIndicatorDist{2};
    uint8_t programIndicatorWidth = (m_display.width() / m_bankSize) - (programIndicatorDist * 2);
    uint8_t offset = programIndicatorWidth + (programIndicatorDist * 2);

    for (auto i{0}; i < m_bankSize; ++i)
    {
        if (i == m_lastProgram)
        {
            m_display.fillCircle((offset / 2) + (offset * i), offset / 2, programIndicatorWidth / 2, SSD1306_WHITE);
        }
        else
        {
            m_display.drawCircle((offset / 2) + (offset * i), offset / 2, programIndicatorWidth / 2, SSD1306_WHITE);
        }
    }

    char bankString[4];
    sprintf(bankString, "%02d", m_lastBank + 1);
    int16_t x1, y1;
    uint16_t w, h;

    m_display.setTextSize(3);
    m_display.getTextBounds(bankString, 0, 0, &x1, &y1, &w, &h);
    m_display.setCursor(0, m_display.height() - h);
    m_display.setTextColor(SSD1306_WHITE);
    m_display.print(bankString);

    uint8_t absoluteProgram = (m_bankSize * m_lastBank) + m_lastProgram + 1;
    char programString[5];
    sprintf(programString, "%03d", absoluteProgram);

    // m_display.setTextSize(1);
    m_display.getTextBounds(programString, 0, 0, &x1, &y1, &w, &h);
    m_display.setCursor(m_display.width() - w, m_display.height() - h);
    m_display.print(programString);

    m_display.display();
}