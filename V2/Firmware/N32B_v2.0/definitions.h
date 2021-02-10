/*
  N32B Firmware v2.1
  MIT License

  Copyright (c) 2021 SHIK
*/

/*
 * Definitions of constats
 */

#ifndef N32B_DEFINITIONS
#define N32B_DEFINITIONS

/* Pin setup */
#define MUX_A_SIG (const uint8_t)8
#define MUX_B_SIG (const uint8_t)9
#define MIDI_TX_PIN (const uint8_t)1
#define MUX_S0 (const uint8_t)2
#define MUX_S1 (const uint8_t)3
#define MUX_S2 (const uint8_t)4
#define MUX_S3 (const uint8_t)5
#define LED_PIN (const uint8_t)17
#define BUTTON_A_PIN (const uint8_t) A3
#define BUTTON_B_PIN (const uint8_t) A2

// Reset to factory preset timeout
#define reset_timeout (unsigned int)4000

/*--- EEPROM Format Chuncks ---*/
// Change these any time the data structure of a preset changed
// This will trigger reformatting on the next startup
#define MAJOR_VERSION (const int)0x02
#define MINOR_VERSION (const int)0x00
#define POINT_VERSION (const int)0x01

// SYSEX constants
// const uint8_t SHIK_MANUFACTURER_ID = 32;
#define SHIK_MANUFACTURER_ID (const uint8_t)124

#define MANUFACTURER (const uint8_t)1
#define COMMAND (const uint8_t)2
#define KNOB_INDEX (const uint8_t)3
#define MSB_INDEX (const uint8_t)4
#define LSB_INDEX (const uint8_t)5
#define CHANNEL_INDEX (const uint8_t)6
#define MIN_VALUE_INDEX (const uint8_t)7
#define MAX_VALUE_INDEX (const uint8_t)8

#define SET_KNOB_AS_CC (const uint8_t)1         // CC
#define SET_KNOB_AS_CC_CHANNEL (const uint8_t)2 // CC & Channel
#define SET_KNOB_AS_NRPN (const uint8_t)3       // NRPN
#define SAVE_PRESET (const uint8_t)5            // Save the preset
#define LOAD_PRESET (const uint8_t)6            // Load a preset
#define SEND_CURRENT_CONFIG (const uint8_t)7    // Send the current config
#define SYNC_KNOBS (const uint8_t)8             // Forces the emission of the messages associated to every knob
#define CHANGE_CHANNEL (const uint8_t)9         // Changes the global MIDI channel
#define DISABLE_KNOB (const uint8_t)11          // Disable
#define HIGH_RES_14BIT (const uint8_t)14        // Use 7-bit or 14-bit midi messages


// General definitions
#define NUMBER_OF_KNOBS (const uint8_t)32
#define NUMBER_OF_PRESETS (const uint8_t)5

#endif
