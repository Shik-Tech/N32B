/*
  N32B Firmware v2.1
  MIT License

  Copyright (c) 2021 SHIK
*/

// void readEEPROMArray(uint16_t baseAddress, uint8_t buffer[], int length)
// {
//   for (uint16_t byteIndex = 0; byteIndex < length; byteIndex++)
//   {
//     buffer[byteIndex] = EEPROM.read(baseAddress + byteIndex);
//   }
// }

// void writeEEPROMArray(int start, byte buffer[], int length) {
//   for (int i = 0; i < length; i++) {
//     EEPROM.write(start+i, buffer[i]);
//   }
// }

void printHex(uint8_t num)
{
  char hexCar[2];

  sprintf(hexCar, "%02X", num);
  Serial.print(hexCar);
}

void printHexArray(byte *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printHex(array[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void printIntArray(int *array, int size)
{
  for (int i = 0; i < size; i++)
  {
    printHex(array[i]);
    Serial.print(" ");
  }
  Serial.println();
}