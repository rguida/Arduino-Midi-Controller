#pragma once

#include <Adafruit_SSD1306.h>
#include "IMode.h"

struct MenuItem
{
    uint8_t id;
    uint8_t parentId;
    char text[12];
};

enum class Setting : uint8_t
{
    FunctionMode,
    NumOfProgramsPerBank
};

class ProgramMode : public IMode
{
public:
    ProgramMode(Adafruit_SSD1306 &display);

    void Update() override;

    void SetSwitch1(bool value) override;

    void SetSwitch2(bool value) override;

    void Increment() override;

    void Decrement() override;

    void Select() override;

private:
    static constexpr uint8_t numOfMenuItems{7};
    static constexpr MenuItem m_menuItems[numOfMenuItems] =
        {
            {0x00, 0xff, "Func Mode"}
          , {0x01, 0xff, "Live #Progs"}
          , {0x11, 0x00, "Live"}
          , {0x12, 0x00, "Looper"}
          , {0x21, 0x01, "4 prgs/bank"}
          , {0x22, 0x01, "6 prgs/bank"}
          , {0x23, 0x01, "8 prgs/bank"}
        };

    static constexpr uint8_t numOfSettings{2};
    uint8_t m_settingValues[numOfSettings] = {0x01, 0x01};

    Adafruit_SSD1306 m_display;
    uint8_t m_currentMenuItem{0x01};
    uint8_t m_currentParentId{0xff};
    bool m_isChanged{false};

    void UpdateMenuItem(uint8_t);
};