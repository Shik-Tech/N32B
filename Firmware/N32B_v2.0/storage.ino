//checks if the machine starts up for the first time and loads the factory presets if needed
bool isEEPROMvalid() {
  //we check if this is the first startup of the machine
  uint8_t byte1, byte2, byte3;

  byte1 = EEPROM.read(EEPROM.length() - 3);
  byte2 = EEPROM.read(EEPROM.length() - 2);
  byte3 = EEPROM.read(EEPROM.length() - 1);

  //if this is the first time this device is powered on
  return ((byte1 == sigByteOne) && (byte2 == sigByteTwo) && (byte3 == sigByteThree));
}

void formatFactory() {
  
  // set last used preset to number 0
  EEPROM.update(lastUsedPresetAddress, 0);
  
  // Create default preset
  Preset_t defaultPreset;
  defaultPreset.channel = 1;

    defaultPreset.knobInfo[0].CC = 84;
    defaultPreset.knobInfo[0].NRPN = 0;
    defaultPreset.knobInfo[0].SYSEX = 128;
    defaultPreset.knobInfo[1].CC = 83;
    defaultPreset.knobInfo[1].NRPN = 0;
    defaultPreset.knobInfo[1].SYSEX = 128;
    defaultPreset.knobInfo[2].CC = 51;
    defaultPreset.knobInfo[2].NRPN = 0;
    defaultPreset.knobInfo[2].SYSEX = 128;
    defaultPreset.knobInfo[3].CC = 75;
    defaultPreset.knobInfo[3].NRPN = 0;
    defaultPreset.knobInfo[3].SYSEX = 128;
    defaultPreset.knobInfo[4].CC = 71;
    defaultPreset.knobInfo[4].NRPN = 0;
    defaultPreset.knobInfo[4].SYSEX = 128;
    defaultPreset.knobInfo[5].CC = 103;
    defaultPreset.knobInfo[5].NRPN = 0;
    defaultPreset.knobInfo[5].SYSEX = 128;
    defaultPreset.knobInfo[6].CC = 109;
    defaultPreset.knobInfo[6].NRPN = 0;
    defaultPreset.knobInfo[6].SYSEX = 128;
    defaultPreset.knobInfo[7].CC = 89;
    defaultPreset.knobInfo[7].NRPN = 0;
    defaultPreset.knobInfo[7].SYSEX = 128;
    defaultPreset.knobInfo[8].CC = 51 | 0x80;
    defaultPreset.knobInfo[8].NRPN = 0 | 0x80;
    defaultPreset.knobInfo[8].SYSEX = 128 + 11;
    defaultPreset.knobInfo[9].CC = 122;
    defaultPreset.knobInfo[9].NRPN = 0;
    defaultPreset.knobInfo[9].SYSEX = 128;
    defaultPreset.knobInfo[10].CC = 86;
    defaultPreset.knobInfo[10].NRPN = 0;
    defaultPreset.knobInfo[10].SYSEX = 128;
    defaultPreset.knobInfo[11].CC = 85;
    defaultPreset.knobInfo[11].NRPN = 0;
    defaultPreset.knobInfo[11].SYSEX = 128;
    defaultPreset.knobInfo[12].CC = 53;
    defaultPreset.knobInfo[12].NRPN = 0;
    defaultPreset.knobInfo[12].SYSEX = 128;
    defaultPreset.knobInfo[13].CC = 76;
    defaultPreset.knobInfo[13].NRPN = 0;
    defaultPreset.knobInfo[13].SYSEX = 128;
    defaultPreset.knobInfo[14].CC = 39;
    defaultPreset.knobInfo[14].NRPN = 0;
    defaultPreset.knobInfo[14].SYSEX = 128;
    defaultPreset.knobInfo[15].CC = 45;
    defaultPreset.knobInfo[15].NRPN = 0;
    defaultPreset.knobInfo[15].SYSEX = 128;
    defaultPreset.knobInfo[16].CC = 110;
    defaultPreset.knobInfo[16].NRPN = 0;
    defaultPreset.knobInfo[16].SYSEX = 128;
    defaultPreset.knobInfo[17].CC = 90;
    defaultPreset.knobInfo[17].NRPN = 0;
    defaultPreset.knobInfo[17].SYSEX = 128;
    defaultPreset.knobInfo[18].CC = 52 | 0x80;
    defaultPreset.knobInfo[18].NRPN = 0 | 0x80;
    defaultPreset.knobInfo[18].SYSEX = 128 + 11;
    defaultPreset.knobInfo[19].CC = 123;
    defaultPreset.knobInfo[19].NRPN = 0;
    defaultPreset.knobInfo[19].SYSEX = 128;
    defaultPreset.knobInfo[20].CC = 13;
    defaultPreset.knobInfo[20].NRPN = 0;
    defaultPreset.knobInfo[20].SYSEX = 128;
    defaultPreset.knobInfo[21].CC = 87;
    defaultPreset.knobInfo[21].NRPN = 0;
    defaultPreset.knobInfo[21].SYSEX = 128;
    defaultPreset.knobInfo[22].CC = 55;
    defaultPreset.knobInfo[22].NRPN = 0;
    defaultPreset.knobInfo[22].SYSEX = 128;
    defaultPreset.knobInfo[23].CC = 77;
    defaultPreset.knobInfo[23].NRPN = 0;
    defaultPreset.knobInfo[23].SYSEX = 128;
    defaultPreset.knobInfo[24].CC = 40;
    defaultPreset.knobInfo[24].NRPN = 0;
    defaultPreset.knobInfo[24].SYSEX = 128;
    defaultPreset.knobInfo[25].CC = 46;
    defaultPreset.knobInfo[25].NRPN = 0;
    defaultPreset.knobInfo[25].SYSEX = 128;
    defaultPreset.knobInfo[26].CC = 111;
    defaultPreset.knobInfo[26].NRPN = 0;
    defaultPreset.knobInfo[26].SYSEX = 128;
    defaultPreset.knobInfo[27].CC = 91;
    defaultPreset.knobInfo[27].NRPN = 0;
    defaultPreset.knobInfo[27].SYSEX = 128;
    defaultPreset.knobInfo[28].CC = 53 | 0x80;
    defaultPreset.knobInfo[28].NRPN = 0 | 0x80;
    defaultPreset.knobInfo[28].SYSEX = 128 + 11;
    defaultPreset.knobInfo[29].CC = 124;
    defaultPreset.knobInfo[29].NRPN = 0;
    defaultPreset.knobInfo[29].SYSEX = 128;
    defaultPreset.knobInfo[30].CC = 15;
    defaultPreset.knobInfo[30].NRPN = 0;
    defaultPreset.knobInfo[30].SYSEX = 128;
    defaultPreset.knobInfo[31].CC = 29;
    defaultPreset.knobInfo[31].NRPN = 0;
    defaultPreset.knobInfo[31].SYSEX = 128;
  
  defaultPreset.invertBits = 0;
  defaultPreset.dropNRPNLSBvalue = 0;
  
  // we write the default preset to all preset slots, byte by byte
  uint16_t baseAddress = 1;
  for (uint8_t p=0; p<NUMBEROFPRESETS; p++) { 
    for (uint16_t byteIndex=0; byteIndex<sizeof(Preset_t); byteIndex++) {
      EEPROM.update(baseAddress + p*sizeof(Preset_t) + byteIndex, ((uint8_t*)(&defaultPreset))[byteIndex]);
    }
  }
  
  //we write the signature so that the device will never rewrite the factory presets
  EEPROM.update(EEPROM.length() - 3, sigByteOne);
  EEPROM.update(EEPROM.length() - 2, sigByteTwo);
  EEPROM.update(EEPROM.length() - 1, sigByteThree);
}


//Loads the specified preset in the RAM and make it the last used preset
void loadPreset(uint8_t presetNumber) {
  
  uint16_t baseAddress = 1 + (presetNumber * sizeof(Preset_t));

  // read the active preset from EEPROM; byte by byte  
  for (uint16_t byteIndex=0; byteIndex<sizeof(Preset_t); byteIndex++) {
    ((uint8_t*)(&activePreset))[byteIndex] = EEPROM.read(baseAddress + byteIndex);
  }

  //update the last used preset
  currentPresetNumber = presetNumber;
  EEPROM.update(lastUsedPresetAddress, currentPresetNumber);
}



void savePreset(uint8_t presetNbr)
{
  uint16_t baseAddress = presetNbr * sizeof(Preset_t) + 1;

  // write the active preset to EEPROM; byte by byte  
  for (uint16_t byteIndex=0; byteIndex<sizeof(Preset_t); byteIndex++) {
    EEPROM.update(baseAddress + byteIndex, ((uint8_t*)(&activePreset))[byteIndex]);
  }
  
  //Visual feedback
  //we wait a bit with the LED oon
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  //we blink it
  digitalWrite(LED_PIN, LOW);
  delay(250);
  digitalWrite(LED_PIN, HIGH);
  delay(250);
  digitalWrite(LED_PIN, LOW);
}
