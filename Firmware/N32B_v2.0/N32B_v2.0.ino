/*
  N32B Firmware v2.0
  MIT License

  Copyright (c) 2020 SHIK
*/

/* Libraries */
// #include <MIDI.h>
#include <USB-MIDI.h>
#include <EEPROM.h>
#include <elapsedMillis.h>
#include <RoxMux.h>
#include "DigitLedDisplay.h"

#include "definitions.h"
#include "variables.h"
#include "components.h"

/* Pin setup */
/* Display setup: (DIN, CS, CLK) */
DigitLedDisplay display = DigitLedDisplay(16, 10, 15);
N32B_DISPLAY n32b_display;
/* Mux setup */
#define MUX_TOTAL 2
Rox74HC4067<MUX_TOTAL> mux;
RoxPot pot;

#define MUX_A_SIG 8
#define MUX_B_SIG 9
#define MIDI_TX_PIN 1
#define MUX_S0 2
#define MUX_S1 3
#define MUX_S2 4
#define MUX_S3 5
#define LED_PIN 17
#define BUTTON_A_PIN A3
#define BUTTON_B_PIN A2

#define PIN_TO_READ 19

// USBMIDI_CREATE_DEFAULT_INSTANCE();
USBMIDI_CREATE_INSTANCE(4, midiUsb);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiDin);
uint8_t prevLUT[32]; // TEMP for testing

// set an interval between button clicks
elapsedMillis buttonsTimeElapsed;   //declare global if you don't want it reset every time loop runs
uint8_t buttonsInterval = 200; // delay in milliseconds

void updateChannel()
{
  if (buttonsTimeElapsed > buttonsInterval)
  {
    if (digitalRead(BUTTON_A_PIN) == LOW)
    {
      if (currentChannel < 16)
      {
        currentChannel++;
      }
      else
      {
        currentChannel = 1;
      }
      n32b_display.showCurrentChannel(display, currentChannel);
    }
    else if (digitalRead(BUTTON_B_PIN) == LOW)
    {
      if (currentChannel > 1)
      {
        currentChannel--;
      }
      else
      {
        currentChannel = 16;
      }
      n32b_display.showCurrentChannel(display, currentChannel);
    }
    buttonsTimeElapsed = 0;
  }
}

void setup()
{
  Serial.begin(115200);

  /* Set the brightness min:1, max:15 */
  display.setBright(1);

  /* Set amount of digits in the display */
  display.setDigitLimit(2);

  mux.begin(MUX_S0, MUX_S1, MUX_S2, MUX_S3);
  mux.setSignalPin(0, MUX_A_SIG);
  mux.setSignalPin(1, MUX_B_SIG);

  /* Pin setup */
  pinMode(MIDI_TX_PIN, OUTPUT);
  digitalWrite(MIDI_TX_PIN, HIGH); // Prevent random messages on startup
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);

  pinMode(MUX_A_SIG, INPUT);
  // little delay before starting
  delay(100);
  // the .begin() method starts the debouncing timer
  pot.begin();

  /* Initiate MIDI communications, listen to all channels */
  midiUsb.begin(MIDI_CHANNEL_OMNI);
  midiDin.begin(MIDI_CHANNEL_OMNI);

  // Send an Active Sensing MIDI message to notify the target that the controller is on the bus
  // midiUsb.sendRealTime((midi::MidiType)0xFE);
  midiDin.sendRealTime((midi::MidiType)0xFE);
}

void loop()
{
  // MIDI.read();
  // selectKnob(currentKnob); //Sets up the MUXs to direct the right knob to the analog input
  // MIDI.read();
  // updateKnob(currentKnob); //Read the current knob value and update the buffers
  // MIDI.read();
  // interpretKnob(currentKnob, false, false); //send the data if needed
  updateChannel();
  mux.update();

  // if (midiDin.read()) // Is there a MIDI message incoming ?
  // {
  //   switch (midiDin.getType()) // Get the type of the message we caught
  //   {
  //   case midi::ProgramChange: // If it is a Program Change,
  //     Serial.print("READ: ");
  //     Serial.println(midiDin.getData1());
  //     break;
  //   // See the online reference for other message types
  //   default:
  //     break;
  //   }
  // }

  for (uint8_t i = 0, n = MUX_TOTAL * 16; i < n; i++)
  {
    uint8_t currentKnob = knobLUT[i];
    updateKnob(currentKnob);
    if (prevLUT[currentKnob] != getKnobValue(currentKnob))
    {
      prevLUT[currentKnob] = getKnobValue(currentKnob);
      Serial.print("Mux pin ");
      Serial.print(currentKnob);
      Serial.print(" = ");
      Serial.println(prevLUT[currentKnob]);

      midiUsb.sendControlChange(currentKnob, prevLUT[currentKnob], currentChannel);
      midiDin.sendControlChange(currentKnob, prevLUT[currentKnob], currentChannel);

      n32b_display.displayAnimation(display, prevLUT[currentKnob]);
    }
  }
  n32b_display.showCurrentChannel(display, currentChannel);

  // if(pot.update(analogRead(MUX_A_SIG), 1)){
  //   uint8_t newPotReading = pot.read();
  //   Serial.print("Pot value: ");
  //   Serial.println(newPotReading);
  // }
}