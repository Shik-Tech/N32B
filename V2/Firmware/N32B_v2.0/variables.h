/*
  N32B Firmware v2.0
  MIT License

  Copyright (c) 2021 SHIK
*/

#ifndef _VARIABLES_H_
#define _VARIABLES_H_

/*
*  A preset stores all the information that
*  define how the device operates
*/
struct Preset_t
{

  // channel the device is sending on
  uint8_t channel;

  // Knob settings structure
  struct Knob_t
  {
    uint8_t MSB;
    uint8_t LSB;
    uint8_t minValue;
    uint8_t maxValue;
    uint8_t CHANNEL;
    bool NRPN;
  } knobInfo[NUMBER_OF_KNOBS];

  // High resolution 14-bit
  bool highResolution;
};

/* Device setup data */

byte currentPresetNumber;
Preset_t activePreset;

unsigned int knobBuffer[4][32];
unsigned int emittedValue[3][32];

/* Buttons variables */
const unsigned int SHORT_PRESS_TIME = 1000; // 1 Second
const unsigned int LONG_PRESS_TIME  = 1500; // 1.5 Seconds
unsigned long pressedTime  = 0;
// unsigned long releasedTime = 0;
bool isPressingAButton = false;
bool isPressingBButton = false;
bool isLongDetected = false;
bool isPresetMode = false;

bool wasFactoryReset;
bool wasStartingUp = 0;
// byte index in EEPROM for the last used preset
const uint8_t lastUsedPresetAddress = 0;

#endif
