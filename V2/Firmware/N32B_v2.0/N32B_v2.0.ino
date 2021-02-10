/*
  N32B Firmware v2.0
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
#include "mux.h"

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

/* Mux setup */
MUX mux;

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

  mux.init(MUX_S0, MUX_S1, MUX_S2, MUX_S3);
  mux.setSignalPin(0, MUX_A_SIG);
  mux.setSignalPin(1, MUX_B_SIG);

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
void onUsbMessage(const MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport>::MidiMessage &message)
{
  MIDICoreSerial.send(message);
  n32b_display.blinkDot(1);
}

void onSerialMessage(const MidiInterface<SerialMIDI<HardwareSerial> >::MidiMessage &message)
{
  // MIDICoreUSB.send(message);
  MIDICoreUSB.sendControlChange(message.data1, message.data2, message.channel);
  n32b_display.blinkDot(1);
}
// void onUsbMessage(const MidiMessage& message)
// {
//   MIDICoreSerial.send(message);
// }

// void onSerialMessage(const MidiMessage& message)
// {
//   MIDICoreUSB.send(message);
// }

void loop()
{

  /* Show factory reset animation */
  if (wasFactoryReset)
  {
    n32b_display.factoryResetAnimation();
    wasFactoryReset = false;
  }

  mux.update(doMidiRead);
  // updateKnob();
  // Iterate all knobs, read update buffers and send
  for (uint8_t currentKnob = 0; currentKnob < NUMBER_OF_KNOBS; currentKnob++)
  {
    // doMidiRead();                             // Read
    // updateKnob(currentKnob);                  // Update buffers
    // doMidiRead();                             // Read
    interpretKnob(currentKnob, false, false); // Send
  }

  // n32b_display.renderDisplay(activePreset); // Render the display
  n32b_display.updateDisplay();
  renderFunctionButton();                   // Update buttons stats
}