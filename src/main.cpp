/*

MIDI Controller

Sends MIDI Program Change commands via software serial.

INPUT
    a) Foot Switch 1
    b) Foot Switch 2
    c) Rotary Encoder
    d) Rotary Encoder Switch
    e) Three-position Switch (Function-OFF-Program)

OUTPUT
    f) OLED Display
    g) MIDI port

MODES
- Function
    It runs the select function mode
    Live
       a) selects the previous program within the bank; the last one if the current program is the first one
       b) selects the next program within the bank; the first one if the current program is the last one
       c) selects the previous/next bank

    Looper
       a) Record/Overwrite/Play bank 1
       b) Record/Overwrite/Play bank 2

- Program
    Implements a menu system.
    c) Navigate to next/previous item
    d) Confirm selection

    The selection is immediately saved in EEPROM.

    Mode
        Live
        Looper
    Live Programs
        4
        6
        8
*/

#include <Adafruit_SSD1306.h>
#include "DebugHelpers.h"
#include "LiveMode.h"
#include "ProgramMode.h"
#include "RotaryEncoder.h"

namespace
{
  constexpr uint8_t DisplayWidth{128};
  constexpr uint8_t DisplayHeight{64};
  constexpr uint8_t OLED_RESET{4}; // Reset pin # (or -1 if sharing Arduino reset pin)
  Adafruit_SSD1306 m_display{DisplayWidth, DisplayHeight, &Wire, OLED_RESET};

  constexpr uint8_t ENCODER_CLOCK{2};
  constexpr uint8_t ENCODER_DATA{3};
  constexpr uint8_t ENCODER_SWITCH{4};

  constexpr int8_t NumberInitialValue{0};

  constexpr uint8_t NumOfPrograms{128};
  constexpr uint8_t BankSize{8};
  constexpr uint8_t NumOfBanks{NumOfPrograms / BankSize};

  constexpr uint8_t Switch1Pin{5};
  constexpr uint8_t Switch2Pin{6};

  RotaryEncoder m_rotaryEncoder{ENCODER_CLOCK, ENCODER_DATA, ENCODER_SWITCH};
  bool m_lastEncoderSwitchState;
  // LiveMode m_currentModeHandler{m_display};
  IMode *m_currentModeHandler;

}

void setup()
{
#ifdef DEBUG
  // debug_init();
  Serial.begin(9600);
  while (!Serial && !Serial.available())
  {
  }

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
#else
  // Log.begin(LOG_LEVEL_SILENT, &Serial, false);
#endif

  mylog((F("Start")));
  mylogmem();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  mylog(F("Clear the display buffer."));
  m_display.clearDisplay();
  m_display.display();
  mylogmem();

  m_currentModeHandler = new ProgramMode(m_display);
  mylogmem();
}

void loop()
{
  mylogmem(F("Loop"));

  m_rotaryEncoder.Update();

  if (m_lastEncoderSwitchState != m_rotaryEncoder.GetSwitchState())
  {
    m_lastEncoderSwitchState = m_rotaryEncoder.GetSwitchState();
    if (m_rotaryEncoder.GetSwitchState())
    {
      m_currentModeHandler->Select();
    }
  }

  if (m_rotaryEncoder.GetRotationState() == RotationState::Clockwise)
  {
    m_currentModeHandler->Increment();
  }

  if (m_rotaryEncoder.GetRotationState() == RotationState::CounterClockwise)
  {
    m_currentModeHandler->Decrement();
  }

  m_currentModeHandler->Update();

  delay(1);
}
