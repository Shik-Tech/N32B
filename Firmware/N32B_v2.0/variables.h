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

  // information on how knobs are setup
  struct Knob_t
  {
    uint8_t CC;
    uint8_t NRPN;
    uint8_t SYSEX;
  } knobInfo[NUMBEROFKNOBS];

  // compressed bitarray also for knobs setup
  uint64_t invertBits;

  // should NRPN LSB be not sent if it is zero
  bool dropNRPNLSBvalue;
};

/* Device setup data */

uint8_t currentPresetNumber;
Preset_t activePreset;
uint8_t currentChannel = 1;

/* Hardware data */
const uint8_t knobLUT[32] =
    {
        15, 14, 15, 9, 31, 24, 15, 16,
        13, 11, 10, 2, 30, 25, 21, 17,
        8, 15, 4, 1, 29, 26, 15, 18,
        15, 15, 3, 0, 28, 27, 23, 19};

// const uint8_t knobLUT[32] =
//     {
//         15, 14, 12, 9, 31, 24, 20, 16,
//         13, 11, 10, 2, 30, 25, 21, 17,
//         8, 7, 4, 1, 29, 26, 22, 18,
//         6, 5, 3, 0, 28, 27, 23, 19};

uint8_t knobBuffer[4][32];
uint8_t emittedValue[3][32];
unsigned long lastButtonPress; //stores the timestamp of the last button press

// byte index in EEPROM for the last used preset
const uint8_t lastUsedPresetAddress = 0;

#endif
