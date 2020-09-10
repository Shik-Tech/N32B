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

/* Pin setup */
/* Display setup: (DIN, CS, CLK) */
DigitLedDisplay display = DigitLedDisplay(16, 10, 15);

/* Mux setup */
#define MUX_TOTAL 2
Rox74HC4067<MUX_TOTAL> mux;

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
elapsedMillis buttonsTimeElapsed; //declare global if you don't want it reset every time loop runs
unsigned int buttonsInterval = 200; // delay in milliseconds

void displayAnimation(uint8_t value)
{
  display.clear();
  uint8_t mappedValue = map(value, 0, 127, 0, 9);

  // Full circle
  switch (mappedValue)
  {
  case 0:
    display.clear();
    break;
  case 1:
    display.write(2, B10000000);
    display.write(1, B00000000);
    break;
  case 2:
    display.write(2, B10001000);
    display.write(1, B00000000);
    break;
  case 3:
    display.write(2, B10001100);
    display.write(1, B00000000);
    break;
  case 4:
    display.write(2, B10001110);
    display.write(1, B00000000);
    break;
  case 5:
    display.write(2, B11001110);
    display.write(1, B00000000);
    break;
  case 6:
    display.write(2, B11001110);
    display.write(1, B01000000);
    break;
  case 7:
    display.write(2, B11001110);
    display.write(1, B01100000);
    break;
  case 8:
    display.write(2, B11001110);
    display.write(1, B01110000);
    break;
  case 9:
    display.write(2, B11001110);
    display.write(1, B01111000);
    break;
  }
}

void updateChannel()
{
  if (buttonsTimeElapsed > buttonsInterval)
  {
    if (digitalRead(BUTTON_A_PIN) == LOW)
    {
      if (currentChannel < 16)
      {
        currentChannel++;
      } else {
        currentChannel = 1;
      }
      display.clear();
      display.printDigit(currentChannel);
    }
    else if (digitalRead(BUTTON_B_PIN) == LOW)
    {
      if (currentChannel > 1)
      {
        currentChannel--;
      } else {
        currentChannel = 16;
      }
      display.clear();
      display.printDigit(currentChannel);
    }
    buttonsTimeElapsed = 0;
  }
}

void setup()
{
  Serial.begin(115200);

  /* Set the brightness min:1, max:15 */
  display.setBright(3);

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
  for (uint8_t currentKnob = 0, n = MUX_TOTAL * 16; currentKnob < n; currentKnob++)
  {
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

      displayAnimation(prevLUT[currentKnob]);
    }
  }
}