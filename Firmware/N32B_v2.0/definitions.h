/*
 * This file contains all the definitions of the constats used throughout the code
 */

#ifndef INC_GUARD_DEFINITIONS
#define INC_GUARD_DEFINITIONS

#include <stdint.h>

//reset to factory preset timeout
const uint16_t reset_timeout = 5000;

/*--- EEPROM Format Chuncks ---*/

// change these any time the data structure of a preset changed
// this will trigger reformatting on the next startup
uint8_t sigByteOne = 0xAB;
uint8_t sigByteTwo = 0x3D;
uint8_t sigByteThree = 0x66;


/*---   SYSEX INTERPRETER CONSTANTS   ---*/
const uint8_t BASTL_MANUFACTURER_ID = 48;

enum SysexByteNumber_t {
  MANUFACTURER = 1,
  COMMAND = 2,
  PARAM1 = 3,
  PARAM2 = 4,
  PARAM3 = 5,
  PARAM4 = 6
};

//the numbers associated with every command in the interpreter
enum Command_t {
  SETKNOBASGLOBALCC = 1,
  SETKNOBASINDEPCC = 15,
  SETKNOBASBNRPN = 2,
  SETKNOBASUNRPN = 3,
  SETKNOBASDX = 4,
  PRESETSAVE = 5,
  PRESETLOAD = 6,
  DUMPPRESET = 7,
  SYNCKNOBS = 8,
  CHANNELCHANGE = 9,
  DISABLEKNOB = 16,
  RANDOMIZER = 10,
  INVERTKNOB = 17,
  DROPNRPNLSB = 19,
  SETKNOBASENRPN = 18
};

/*---   Tunable interface behavior   ---*/
const uint8_t KnobSelectThreshold = 2;
//#define CLICKSPEEDMIN 100
//#define CLICKSPEEDMAX 350

/*---   MISC. CONSTANTS   ---*/
const uint8_t NUMBEROFKNOBS = 32;
const uint8_t NUMBEROFPRESETS = 4;

#endif
