/*
  N32B Firmware v2.0
  MIT License

  Copyright (c) 2021 SHIK
*/

// Updates the buffers and samples the analog pin
void updateKnob(uint8_t index)
{
  for (byte i = 3; i > 0; i--)
  {
    knobBuffer[i][index] = knobBuffer[i - 1][index];
  }

  // 10-bit (translated to 14-bit midi message) High resolution
  knobBuffer[0][index] = mux.read(index);
}

// Returns average of previous knob reads
uint16_t getKnobValue(uint8_t index)
{
  uint16_t average = 0;
  for (byte i = 0; i < 4; i++)
  {
    average += knobBuffer[i][index];
  }
  average /= 4;

  return average;
}
