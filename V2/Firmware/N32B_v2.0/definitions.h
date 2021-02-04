/*
  N32B Firmware v2.0
  MIT License

  Copyright (c) 2021 SHIK
*/

/*
 * Definitions of constats
 */

#ifndef INC_GUARD_DEFINITIONS
#define INC_GUARD_DEFINITIONS

// Reset to factory preset timeout
const unsigned int reset_timeout = 4000;

/*--- EEPROM Format Chuncks ---*/
// Change these any time the data structure of a preset changed
// This will trigger reformatting on the next startup
int MAJOR_VERSION = 0x02;
int MINOR_VERSION = 0x00;
int POINT_VERSION = 0x01;
// uint8_t MAJOR_VERSION = 0xAB;
// uint8_t MINOR_VERSION = 0x3D;
// uint8_t POINT_VERSION = 0x66;

/*---   SYSEX INTERPRETER CONSTANTS   ---*/
// const uint8_t SHIK_MANUFACTURER_ID = 32;
const byte SHIK_MANUFACTURER_ID = 124;

enum SysexByteNumber_t
{
  MANUFACTURER = 1,
  COMMAND = 2,
  KNOB_INDEX = 3,
  MSB = 4,
  LSB = 5,
  CHANNEL = 6,
  MIN_VALUE = 7,
  MAX_VALUE = 8
};

// Commands definitions
enum Command_t
{
  SET_KNOB_AS_CC = 1,         // CC
  SET_KNOB_AS_CC_CHANNEL = 2, // CC & Channel
  SET_KNOB_AS_NRPN = 3,       // Extended Unipolar NRPN (0, XX) knob
  SAVE_PRESET = 5,            // Save the preset
  LOAD_PRESET = 6,            // Load a preset
  SEND_CURRENT_CONFIG = 7,    // Send the current config
  SYNC_KNOBS = 8,             // Forces the emission of the messages associated to every knob
  CHANGE_CHANNEL = 9,         // Changes the global MIDI channel
  DISABLE_KNOB = 11,          // Disable
  HIGH_RES_14BIT = 14         // Use 7-bit or 14-bit midi messages
};

const uint8_t NUMBER_OF_KNOBS = 32;
const uint8_t NUMBER_OF_PRESETS = 5;
const uint8_t KnobSelectThreshold = 2;

#endif
