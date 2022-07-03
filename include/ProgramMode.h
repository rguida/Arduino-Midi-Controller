#pragma once

#include <Adafruit_SSD1306.h>
#include "IMode.h"
#include "Settings.h"

namespace 
{
    const char str_0[] PROGMEM{"Func Mode"};
    const char str_1[] PROGMEM{"Prgs/Bank"};
    const char str_2[] PROGMEM{"Live"};
    const char str_3[] PROGMEM{"Looper"};
    const char str_4[] PROGMEM{"Exit Menu"};
    const char str_5[] PROGMEM{"4 prgs"};
    const char str_6[] PROGMEM{"6 prgs"};
    const char str_7[] PROGMEM{"8 prgs"};
    const char str_8[] PROGMEM{"Exit Menu"};

    const char* const string_table[] PROGMEM = 
    {
        str_0, 
        str_1,
        str_2,
        str_3,
        str_4,
        str_5,
        str_6,
        str_7,
        str_8,
    };
}

struct MenuItem
{
    uint8_t id;
    uint8_t parentIndex;
};

class ProgramMode : public IMode
{
public:
    ProgramMode(Adafruit_SSD1306 &display, Settings& settings);

    void Update() override;

    void SetSwitch1(bool value) override;

    void SetSwitch2(bool value) override;

    void Increment() override;

    void Decrement() override;

    void Select() override;

private:

    static constexpr uint8_t numOfMenuItems{9};
    static constexpr MenuItem m_menuItems[numOfMenuItems] =
    {
          {0x00, 0xff}
        , {0x01, 0xff}
        , {0x00, 0x00}
        , {0x01, 0x00}
        , {0xff, 0x00}
        , {0x00, 0x01}
        , {0x01, 0x01}
        , {0x02, 0x01}
        , {0xff, 0x01}
    };

    Adafruit_SSD1306& m_display;
    uint8_t m_currentMenuItemIdx{0};
    uint8_t m_currentParentIdx{Settings::UndefinedValue};
    bool m_isChanged{true};
    Settings& m_settings;

    void UpdateMenuItem(uint8_t);
    char* GetMenuItemText(uint8_t);
    char m_lastMenuItemText[15];
};