/*
  N32B Firmware v2.1
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

byte currentPresetNumber = 0;
Preset_t activePreset;

// unsigned int knobBuffer[4][32];
unsigned int knobBuffer[4][32];
unsigned int emittedValue[3][32];

/* Buttons variables */
#define SHORT_PRESS_TIME (const unsigned int)600 // Milliseconds
#define LONG_PRESS_TIME (const unsigned int)1000 // Milliseconds
unsigned long pressedTime = 0;
bool isPressingAButton = false;
bool isPressingBButton = false;

/* Mode variables */
bool startUp = true;
bool isPresetMode = false;
bool inhibitMidi = false;

bool wasFactoryReset = false;
// byte index in EEPROM for the last used preset
#define lastUsedPresetAddress (uint8_t)0

#endif
