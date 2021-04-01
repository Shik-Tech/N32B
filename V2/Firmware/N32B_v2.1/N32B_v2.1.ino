/*
  N32B Firmware v2.1
  MIT License

  Copyright (c) 2021 SHIK
*/

/* Libraries */
#include <USB-MIDI.h>
#include <EEPROM.h>
#include <ezButton.h>
#include "DigitLedDisplay.h"

#include "definitions.h"
#include "variables.h"
#include "display.h"
#include "mux_factory.h"

/* Mux setup */
MUX_FACTORY muxFactory;

N32B_DISPLAY n32b_display;

ezButton buttonA(BUTTON_A_PIN);
ezButton buttonB(BUTTON_B_PIN);

USING_NAMESPACE_MIDI;

USBMIDI_CREATE_INSTANCE(0, MIDICoreUSB);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDICoreSerial);

void setup()
{
  // Serial.begin(115200);

  n32b_display.setBright(1);     // Set the display brightness (1-15)
  n32b_display.setDigitLimit(2); // Set amount of digits in the display

  muxFactory.init(MUX_S0, MUX_S1, MUX_S2, MUX_S3);
  muxFactory.setSignalPin(0, MUX_A_SIG);
  muxFactory.setSignalPin(1, MUX_B_SIG);

  /* Pin setup */
  pinMode(MIDI_TX_PIN, OUTPUT);
  digitalWrite(MIDI_TX_PIN, HIGH); // Prevent random messages on startup
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(MUX_A_SIG, INPUT);
  pinMode(MUX_B_SIG, INPUT);

  // Set debounce time to 50 milliseconds
  buttonA.setDebounceTime(50);
  buttonB.setDebounceTime(50);

  /*
  * Factory Reset
  * Hold button-A down while powering the device will reset the presets
  */
  if (!digitalRead(BUTTON_A_PIN))
  {
    bool buttonPressed = true;
    digitalWrite(LED_PIN, LOW);

    while (millis() < reset_timeout) // Check if button has been released before timeout
    {
      if (digitalRead(BUTTON_A_PIN))
      {
        buttonPressed = false;
        break;
      }
    }

    // If button is still held down, then clear eeprom
    if (buttonPressed)
    {
      wasFactoryReset = true;
      // Blink once if reset request has been accepted
      digitalWrite(LED_PIN, HIGH);
      delay(20);
      digitalWrite(LED_PIN, LOW);

      // Clean eeprom
      for (int i = 0; i < EEPROM.length(); i++)
      {
        EEPROM.write(i, 0);
      }
      digitalWrite(LED_PIN, HIGH);
    }
  }

  // Write the factory presets to memory if the device was turn on for the first time
  if (!isEEPROMvalid())
  {
    for (uint8_t i = 0; i < 5; i++)
    {
      digitalWrite(LED_PIN, HIGH);
      delay(300);
      digitalWrite(LED_PIN, LOW);
      delay(300);
    }
    formatFactory();
  }

  // Load the last used preset as stored in EEPROM
  loadPreset(EEPROM.read(lastUsedPresetAddress));

  // We don't want any incorrect data sent at startup so we fill the buffers
  // for (uint8_t i = 0; i < NUMBER_OF_KNOBS; i++)
  // {
  //   updateKnob(i);                 // Update the buffers
  //   interpretKnob(i, false, true); // Fill the emission buffers but do not send midi data
  // }

  /* Set callbacks */
  MIDICoreUSB.setHandleMessage(onUsbMessage);
  MIDICoreSerial.setHandleMessage(onSerialMessage);

  MIDICoreUSB.setHandleSystemExclusive(processSysex);
  // MIDICoreSerial.setHandleSystemExclusive(processSysex);

  MIDICoreUSB.setHandleProgramChange(handleProgramChange);
  MIDICoreSerial.setHandleProgramChange(handleProgramChange);

  /* Initiate MIDI communications, listen to all channels */
  MIDICoreUSB.begin(MIDI_CHANNEL_OMNI);
  MIDICoreSerial.begin(MIDI_CHANNEL_OMNI);

  MIDICoreUSB.turnThruOn();
  MIDICoreSerial.turnThruOn();

  // Send an Active Sensing MIDI message to notify the target that the controller is on the bus
  MIDICoreUSB.sendRealTime((midi::MidiType)0xFE);
  MIDICoreSerial.sendRealTime((midi::MidiType)0xFE);
}

void loop()
{
  /* Show factory reset animation */
  if (wasFactoryReset)
  {
    n32b_display.factoryResetAnimation();
    wasFactoryReset = false;
  }
  if (startUp)
  {
    n32b_display.showStartUpAnimation();
    n32b_display.showChannelNumber(activePreset.channel);
    startUp = false;
  }

  muxFactory.update(doMidiRead);

  // Iterate all knobs and send midi messages
  for (uint8_t currentKnob = 0; currentKnob < NUMBER_OF_KNOBS; currentKnob++)
  {
    interpretKnob(currentKnob, false, inhibitMidi);
  }

  n32b_display.updateDisplay();
  renderButtonFunctions(); // Update buttons stats

  // Inhibit sending messages while changing channel
  // This will prevent unchanged values from being fired
  if (millis() - pressedTime > 500)
  {
    inhibitMidi = false;
  }
}