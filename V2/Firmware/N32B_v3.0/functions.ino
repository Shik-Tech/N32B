/*
  N32B Firmware v3.0
  MIT License

  Copyright (c) 2021 SHIK
*/

// void onUsbMessage(const MidiMessage& message)
void onUsbMessage(const MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>::MidiMessage &message)
{
  MIDICoreSerial.send(message);
  n32b_display.blinkDot(1);
}

// void onSerialMessage(const MidiMessage& message)
void onSerialMessage(const MidiInterface<SerialMIDI<HardwareSerial>>::MidiMessage &message)
{
  // MIDICoreUSB.sendControlChange(message.data1, message.data2, message.channel);
  if(MIDICoreSerial.getType() < 254) {
    MIDICoreUSB.send(message.type, message.data1, message.data2, message.channel);
    n32b_display.blinkDot(1);
  }
}

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
        // Check if it is channel specific message
        uint8_t knobChannel = activePreset.knobInfo[index].CHANNEL & 0x7f;
        if (knobChannel > 0 && knobChannel < 17)
        {
          sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, knobChannel);
        }
        else if (knobChannel == 0)
        {
          sendCCMessage(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, activePreset.channel);
        }
        n32b_display.blinkDot(2);
      }
    }
    else
    {
      // NRPN Message
      sendNRPM(activePreset.knobInfo[index].MSB, activePreset.knobInfo[index].LSB, toSend, activePreset.channel);
      n32b_display.blinkDot(2);
    }

    // Fill the emission buffers
    for (uint8_t i = 2; i > 0; i--)
    {
      emittedValue[i][index] = emittedValue[i - 1][index];
    }
    emittedValue[0][index] = toSend;
  }
}

void sendCCMessage(uint8_t MSB, uint8_t LSB, uint16_t value, uint8_t channel)
{
  if (activePreset.highResolution)
  {
    unsigned int shiftedValue = map(value, 0, 1023, 0, 16383) << 1;

    MIDICoreSerial.sendControlChange(MSB, highByte(shiftedValue), channel);
    MIDICoreSerial.sendControlChange(LSB, lowByte(shiftedValue) >> 1, channel);

    MIDICoreUSB.sendControlChange(MSB, highByte(shiftedValue), channel);
    MIDICoreUSB.sendControlChange(LSB, lowByte(shiftedValue) >> 1, channel);
  }
  else
  {
    // uint8_t lowResSend = map(value, 0, 1023, 0, 127);
    MIDICoreSerial.sendControlChange(MSB, value, channel);
    MIDICoreUSB.sendControlChange(MSB, value, channel);
  }
}

void sendNRPM(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int16_t value, uint8_t channel)
{
  unsigned int shiftedValue = map(value, 0, 1023, 0, 16383) << 1;

  MIDICoreSerial.sendControlChange(99, NRPNNumberMSB & 0x7F, channel); // NRPN MSB
  MIDICoreUSB.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);    // NRPN MSB

  MIDICoreSerial.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); // NRPN LSB
  MIDICoreUSB.sendControlChange(98, NRPNNumberLSB & 0x7F, channel);    // NRPN LSB

  MIDICoreSerial.sendControlChange(6, highByte(shiftedValue), channel); // Data Entry MSB
  MIDICoreUSB.sendControlChange(6, highByte(shiftedValue), channel);    // Data Entry MSB

  if (activePreset.highResolution)
  {
    MIDICoreSerial.sendControlChange(38, lowByte(shiftedValue) >> 1, channel); // LSB for Control 6 (Data Entry)
    MIDICoreUSB.sendControlChange(38, lowByte(shiftedValue) >> 1, channel);    // LSB for Control 6 (Data Entry)
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
  // MIDICoreSerial.sendProgramChange(currentPresetNumber, 1);
  // MIDICoreUSB.sendProgramChange(currentPresetNumber, 1);
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
    inhibitMidi = true;
    if (isPresetMode)
    {
      changePreset(direction);
      n32b_display.showPresetNumber(currentPresetNumber);
    }
    else
    {
      changeChannel(direction);
      n32b_display.showChannelNumber(activePreset.channel);
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

void renderButtonFunctions()
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
      n32b_display.showChannelNumber(activePreset.channel);
    }
    if (isPressingBButton)
    {
      isPresetMode = true;
      n32b_display.showPresetNumber(currentPresetNumber);
    }
  }
}

void doMidiRead()
{
  MIDICoreSerial.read();
  MIDICoreUSB.read();
}

uint16_t getKnobValue(uint8_t index)
{
  uint16_t average = 0;
  for (uint8_t i = 0; i < 5; i++)
  {
    average += knobBuffer[i][index];
  }
  average /= 5;

  return average;
}