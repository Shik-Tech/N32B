/*
  N32B Firmware v2.0
  MIT License

  Copyright (c) 2021 SHIK
*/

void interpretKnob(uint8_t index, bool force, bool inhibit)
{
  // Read knob value
  uint16_t toSend = getKnobValue(index);
  // If the value to send is relevant, send it to the MIDI OUT port
  if (((toSend != emittedValue[0][index]) && (toSend != emittedValue[1][index]) && (toSend != emittedValue[2][index])) || (force == true))
  {
    // CC or NRPN?
    if (activePreset.knobInfo[index].NRPN == 0)
    {
      // CC Message
      if (!inhibit)
      {
        // Check if channel is defined for this knob
        uint8_t knobChannel = activePreset.knobInfo[index].CHANNEL & 0x7f;
        if (knobChannel > 0 && knobChannel < 17)
        {
          sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, knobChannel);
        }
        else if (knobChannel == 0)
        {
          sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, activePreset.channel);
        }
      }
    }
    else
    {
      // NRPN Message
      sendNRPM(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, activePreset.channel);
    }

    n32b_display.valueAnimation(toSend);

    // Fill the emission buffers
    for (uint8_t i = 2; i > 0; i--)
    {
      emittedValue[i][index] = emittedValue[i - 1][index];
    }
    emittedValue[0][index] = toSend;
  }
}

void sendCCMessage(byte MSB, byte LSB, uint16_t value, byte channel)
{
  if (activePreset.highResolution)
  {
    unsigned int mappedValue = map(value, 0, 1023, 0, 16383);
    unsigned int shiftedValue = mappedValue << 1;
    MIDICoreSerial.sendControlChange(MSB, highByte(shiftedValue), channel);
    MIDICoreSerial.sendControlChange(LSB, lowByte(shiftedValue) >> 1, channel);

    MIDICoreUSB.sendControlChange(MSB, highByte(shiftedValue), channel);
    MIDICoreUSB.sendControlChange(LSB, lowByte(shiftedValue) >> 1, channel);
  }
  else
  {
    byte lowResSend = map(value, 0, 1023, 0, 127);
    MIDICoreSerial.sendControlChange(MSB, lowResSend, channel);
    MIDICoreUSB.sendControlChange(MSB, lowResSend, channel);
  }
}

void sendNRPM(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int16_t value, uint8_t channel)
{

  unsigned int mappedValue = map(value, 0, 1023, 0, 16383);
  unsigned int shiftedValue = mappedValue << 1;

  MIDICoreSerial.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); // NRPN LSB
  MIDICoreUSB.sendControlChange(98, NRPNNumberLSB & 0x7F, channel);    // NRPN LSB

  MIDICoreSerial.sendControlChange(99, NRPNNumberMSB & 0x7F, channel); // NRPN MSB
  MIDICoreUSB.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);    // NRPN MSB

  MIDICoreSerial.sendControlChange(6, (value >> 7) & 0x7f, channel); // Data Entry MSB
  MIDICoreUSB.sendControlChange(6, (value >> 7) & 0x7f, channel);    // Data Entry MSB

  if (activePreset.highResolution)
  {
    MIDICoreSerial.sendControlChange(38, value & 0x7f, channel); // LSB for Control 6 (Data Entry)
    MIDICoreUSB.sendControlChange(38, value & 0x7f, channel);    // LSB for Control 6 (Data Entry)
  }
}

// Handles the "menu" system, what to do when the button is pressed
void changeChannel(bool direction)
{
  if (direction)
  {
    // Next Channel
    if (activePreset.channel < 16)
      activePreset.channel++;
    else
      activePreset.channel = 1;
  }
  else
  {
    if (activePreset.channel > 1)
      activePreset.channel--;
    else
      activePreset.channel = 16;
  }
}

void changePreset(bool direction)
{
  if (direction)
  {
    // Next Preset
    if (currentPresetNumber < 4)
      loadPreset(currentPresetNumber + 1);
    else
      loadPreset(0);
  }
  else
  {
    // Previous Preset
    if (currentPresetNumber > 0)
      loadPreset(currentPresetNumber - 1);
    else
      loadPreset(4);
  }
  MIDICoreSerial.sendProgramChange(currentPresetNumber, 1);
  MIDICoreUSB.sendProgramChange(currentPresetNumber, 1);
}

void buttonReleaseAction(bool direction)
{
  if (direction)
  {
    isPressingAButton = false;
  }
  else
  {
    isPressingBButton = false;
  }

  if (millis() - pressedTime < SHORT_PRESS_TIME)
  {
    if (isPresetMode)
    {
      changePreset(direction);
    }
    else
    {
      changeChannel(direction);
    }
  }

  MIDICoreUSB.turnThruOn();
  MIDICoreSerial.turnThruOn();
}

void buttonPressAction(bool direction)
{
  pressedTime = millis();

  MIDICoreSerial.turnThruOff();
  MIDICoreUSB.turnThruOff();
}

void renderFunctionButton()
{
  // Must call the loop() function first
  buttonA.loop();
  buttonB.loop();

  if (buttonA.isPressed())
  {
    isPressingAButton = true;
    buttonPressAction(1);
  }

  if (buttonB.isPressed())
  {
    isPressingBButton = true;
    buttonPressAction(0);
  }

  if (buttonA.isReleased())
  {
    buttonReleaseAction(1);
  }

  if (buttonB.isReleased())
  {
    buttonReleaseAction(0);
  }

  // Switch between channelMode and presetMode
  if ((isPressingAButton || isPressingBButton) && (millis() - pressedTime > LONG_PRESS_TIME))
  {
    if (isPressingAButton)
    {
      isPresetMode = false;
    }
    if (isPressingBButton)
    {
      isPresetMode = true;
    }
  }
}
