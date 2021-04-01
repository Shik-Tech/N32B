/*
  N32B Firmware v3.0
  MIT License

  Copyright (c) 2021 SHIK
*/

// Checks if the device is starting up for the first time
bool isEEPROMvalid()
{
  uint8_t byte1, byte2, byte3;

  byte1 = EEPROM.read(EEPROM.length() - 3);
  byte2 = EEPROM.read(EEPROM.length() - 2);
  byte3 = EEPROM.read(EEPROM.length() - 1);

  // Is it the first time the device was powered on
  return ((byte1 == MAJOR_VERSION) && (byte2 == MINOR_VERSION) && (byte3 == POINT_VERSION));
}

void formatFactory()
{
  // Set last used preset to number 0
  EEPROM.update(lastUsedPresetAddress, 0);

  // Create default preset
  Preset_t defaultPreset;
  defaultPreset.channel = 1;
  uint8_t knobsLocation[32] = {
      15, 14, 12, 9, 31, 24, 20, 16,
      13, 11, 10, 2, 30, 25, 21, 17,
      8, 7, 4, 1, 29, 26, 22, 18,
      6, 5, 3, 0, 28, 27, 23, 19};
  for (uint8_t indexId = 0; indexId < 32; indexId++)
  {
    defaultPreset.knobInfo[knobsLocation[indexId]].MSB = indexId;
    defaultPreset.knobInfo[knobsLocation[indexId]].LSB = indexId + 32;
    defaultPreset.knobInfo[knobsLocation[indexId]].NRPN = 0;
    defaultPreset.knobInfo[knobsLocation[indexId]].CHANNEL = 128;
  }

  defaultPreset.highResolution = 1;

  // Write the default preset to all preset slots, byte by byte
  uint16_t baseAddress = 1;
  for (uint8_t p = 0; p < NUMBER_OF_PRESETS; p++)
  {
    for (uint16_t byteIndex = 0; byteIndex < sizeof(Preset_t); byteIndex++)
    {
      EEPROM.update(baseAddress + p * sizeof(Preset_t) + byteIndex, ((uint8_t *)(&defaultPreset))[byteIndex]);
    }
  }

  // we write the signature so that the device will never rewrite the factory presets
  EEPROM.update(EEPROM.length() - 3, MAJOR_VERSION);
  EEPROM.update(EEPROM.length() - 2, MINOR_VERSION);
  EEPROM.update(EEPROM.length() - 1, POINT_VERSION);
}

// Loads the specified preset in the RAM and make it the last used preset
void loadPreset(uint8_t presetNumber)
{
  if (presetNumber < 5)
  {

    uint16_t baseAddress = 1 + (presetNumber * sizeof(Preset_t));
    // Read the active preset from EEPROM; byte by byte
    for (uint16_t byteIndex = 0; byteIndex < sizeof(Preset_t); byteIndex++)
    {
      ((uint8_t *)(&activePreset))[byteIndex] = EEPROM.read(baseAddress + byteIndex);
    }

    // Update the last used preset
    currentPresetNumber = presetNumber;
    n32b_display.showPresetNumber(presetNumber);

    // Save current preset as the active preset.
    // Is commented out to prevent EEPROM from over used
    // EEPROM.update(lastUsedPresetAddress, currentPresetNumber);
  }
}

void savePreset(uint8_t presetNumber)
{
  if (presetNumber < 5)
  {
    uint16_t baseAddress = presetNumber * sizeof(Preset_t) + 1;

    // write the active preset to EEPROM; byte by byte
    for (uint16_t byteIndex = 0; byteIndex < sizeof(Preset_t); byteIndex++)
    {
      EEPROM.update(baseAddress + byteIndex, ((uint8_t *)(&activePreset))[byteIndex]);
    }

    n32b_display.showSaveMessage();
  }
}
