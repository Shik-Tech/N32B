// /*
//    This file includes all the code associated to the prototypes defined in "functions.h"
// */

// //Averages the knob position and decides if a message should be outputed, if yes, decodes the current preset and sends the acording message
// void interpretKnob(uint8_t index, bool force, bool inhibit) {

//   // read averaged position of knob
//   uint16_t toSend = getKnobValue(index);

//   //if the value to send is relevant, we send it to the MIDI OUT port
//   if (((toSend != emittedValue[0][index]) && (toSend != emittedValue[1][index]) && (toSend != emittedValue[2][index])) || (force == true)) {  //if a message should be sent

//     //First we detect which kind of knob it is and we emit the data accordingly
//     if (bitRead(activePreset.knobInfo[index].SYSEX, 7) == 1) {
//       if (activePreset.knobInfo[index].NRPN == 0) {
//         /*---   It's a CC knob    ---*/
//         if (!inhibit) {
//           //check the channel of that specific knob
//           uint8_t knobChannel = activePreset.knobInfo[index].SYSEX & 0x7f;
//           if (knobChannel > 0 && knobChannel < 17) {
//             MIDI.sendControlChange(activePreset.knobInfo[index].CC & 0x7f, toSend, knobChannel);
//           }
//           else if (knobChannel == 0) { //if the channel number is 0, the CC will be sent on the global channel
//             MIDI.sendControlChange(activePreset.knobInfo[index].CC, toSend, activePreset.channel);
//           }
//         }
//       }
//       else {
//         /*---   It's an NRPN knob    ---*/
//         //we calculate the range of the current knob
//         uint8_t range = activePreset.knobInfo[index].SYSEX & 0x7f;

//         if (!inhibit) {
//           if ((activePreset.knobInfo[index].NRPN & 0x80) && (activePreset.knobInfo[index].CC & 0x80)) //if the knob is Unipolar NRPN (range : 0~+Max)
//           {
//             sendUnipolarNRPN(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].CC, map(toSend, 0, 127, 0, range), activePreset.channel);
//           }
//           else {  //the knob is Bipolar NRPN (range : -63~+63)
//             if (range < 64) sendBipolarNRPN(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].CC, map(toSend, 0, 127, -range, range), activePreset.channel);
//             else if ( range <= 64 + 4) {
//               switch (range) {
//                 case 64 :
//                   sendExtendedNRPN(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].CC, map(toSend, 0, 127, 0, 164), activePreset.channel);
//                   break;
//                 case 65 :
//                   sendExtendedNRPN(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].CC, map(toSend, 0, 127, 0, 200), activePreset.channel);
//                   break;
//                 case 66 :
//                   sendExtendedNRPN(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].CC, map(toSend, 0, 127, 0, 1600), activePreset.channel);
//                   break;
//                 case 67 :
//                   sendExtendedNRPN(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].CC, map(toSend, 0, 127, 0, 2000), activePreset.channel);
//                   break;
//               }
//             }
//           }
//         }
//       }
//     }
//     else {
//       /*---   It's an DX7 SysEx knob    ---*/
//       sendDX7Message(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].SYSEX, toSend); //comment this line
//     //  sendRefaceDXMessage(activePreset.knobInfo[index].NRPN, activePreset.knobInfo[index].SYSEX, toSend); // and uncomment this line to make the 60 knobs work with reface DX instead
//     }

//     //we fill the emission buffers
//     for (int i = 2; i > 0; i--) {
//       emittedValue[i][index] = emittedValue[i - 1][index];
//     }
//     emittedValue[0][index] = toSend;
//   }
// }

// //Sends an Unipolar NRPN message
// void sendUnipolarNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, uint8_t value, uint8_t channel) {
//   MIDI.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); //NRPN Number LSB
//   MIDI.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);  //NRPN Number MSB
//   MIDI.sendControlChange(6, value, channel);  //NRPN Value
//   if (!activePreset.dropNRPNLSBvalue) {
//     MIDI.sendControlChange(38, 0, channel);  //NRPN Value
//   }
// }

// //Sends a Bipolar NRPN message
// void sendBipolarNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int8_t value, uint8_t channel) {
//   MIDI.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); //NRPN Number LSB
//   MIDI.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);  //NRPN Number MSB

//   //if the value to send is negative, format the data in the right way
//   if (value < 0) {
//     MIDI.sendControlChange(6, 1, channel);  //NRPN MSB negative
//     MIDI.sendControlChange(38, 128 + value, channel); //NRPN MSB negative
//   }
//   //if the value is positive
//   else {
//     MIDI.sendControlChange(38, value, channel);
//   }
// }

// void sendExtendedNRPN(uint8_t NRPNNumberMSB, uint8_t NRPNNumberLSB, int16_t value, uint8_t channel) {
//   MIDI.sendControlChange(98, NRPNNumberLSB & 0x7F, channel); //NRPN Number LSB
//   MIDI.sendControlChange(99, NRPNNumberMSB & 0x7F, channel);  //NRPN Number MSB
//   MIDI.sendControlChange(6, (value >> 8) & 0x7f, channel); //NRPN MSB
//   MIDI.sendControlChange(38, value & 0x7f, channel); //NRPN MSB
// }

// //Sends a DX7-Parameter Change formated Sysex message
// void sendDX7Message(uint8_t paramNBR, uint8_t rangeMax, uint8_t value) {
//   //the array to transmit
//   uint8_t data[5] = {0x43, 0x10, 0x00, 0x00, 0x00};

//   //if the parameter number takes more than seven bits, set the MSBit correctly
//   if (paramNBR > 127) {
//     data[2] = 0x01;
//   }

//   data[3] = paramNBR & 0x7f;
//   data[4] = map(value, 0, 127, 0, rangeMax);

//   //output the Sysex message
//   MIDI.sendSysEx(5, data, false);

//   //sync all the voices
//   MIDI.sendControlChange(127, 0, 1);  //cc 127 on channel 1 (DX7 only sensitive on channel 1)
// }


// void sendRefaceDXMessage(uint8_t paramNBR, uint8_t rangeMax, uint8_t value) {
//   //the array to transmit
//   uint8_t data[9] = {0x43, 0x10, 0x7F, 0x1C, 0x05, 0x30, 0x00, 0x00, 0x00};

//   //if the parameter number takes more than seven bits, set the MSBit correctly
//   if (paramNBR > 35 && paramNBR < 140) {
//     paramNBR -= 36;
//     uint8_t _operator = paramNBR / 26;
//     data[5] = 0x31;
//     data[6] = _operator;
//     data[7] = paramNBR % 26;
//   }
//   else if (paramNBR <= 34) {
//     data[5] = 0x30;
//     data[6] = 0x00;
//     data[7] = paramNBR;
//   }

//   data[8] = value;//map(value, 0, 127, 0, 127);

//   //output the Sysex message
//   MIDI.sendSysEx(9, data, false);

//   //sync all the voices
//   // MIDI.sendControlChange(127, 0, 1);  //cc 127 on channel 1 (DX7 only sensitive on channel 1)
// }




// //Handles the "menu" system, what to do when the button is pressed
// void renderFunctionButton() {
//   if (!digitalRead(BUTTON_A_PIN)) {

//     //We dont want any messages to be transmitted while the button is down
//     MIDI.turnThruOff();

//     //check if the button has been double pressed, if yes : force the emission of the messages associated to each knob
//     if ((abs(millis() - lastButtonPress) > 100) && (abs(millis() - lastButtonPress) < 350)) {
//       //show the user that the double click worked
//       digitalWrite(LED_PIN, LOW);
//       //sync the knobs
//       for (uint8_t i = 0; i < NUMBEROFKNOBS; i++) {
//         interpretKnob(i, true, false);
//       }
//       delay(500); //just for the hell of it, more visual perhaps
//     }

//     //we send an empty sysex message to ensure the connection between the 60K and the target
//     MIDI.sendSysEx(0, NULL, false);

//     //update the timing variable for the double press detection
//     lastButtonPress = millis();

//     while (!digitalRead(BUTTON_A_PIN)) { //while the button is held down

//       //we detect the movement on the knobs we care for (0 to 15 and 50 to 54)
//       for (int currentKnob = 0; currentKnob < NUMBEROFKNOBS; currentKnob++) {
//         MIDI.read();
//         selectKnob(currentKnob);  //Sets up the MUXs to direct the right knob to the analog input
//         MIDI.read();
//         updateKnob(currentKnob);  //fills the buffer, to know which knobs have moved
//       }


//       //do we need to change the channel ?
//       for (uint8_t channelKnob = 0; channelKnob < 16; channelKnob++) {
//         //if one the knobs associated to the MIDI channel selection has moved enough
//         if (abs(knobBuffer[0][channelKnob] - knobBuffer[1][channelKnob]) > KnobSelectThreshold) {
//           activePreset.channel = channelKnob + 1;
//           //give a visual feedback to prove that the channel has changed
//           digitalWrite(LED_PIN, LOW);
//           delay(100);
//           digitalWrite(LED_PIN, HIGH);
//         }
//       }

//       //do we need to change preset ?
//       for (uint8_t presetKnob = 50; presetKnob < 55; presetKnob++) {
//         //if one of the knobs associated with the preset selection has moved enough
//         if (abs(knobBuffer[0][presetKnob] - knobBuffer[1][presetKnob]) > KnobSelectThreshold) {
//           loadPreset(presetKnob - 50);
//           //give a visual feedback to prove that the preset has changed
//           digitalWrite(LED_PIN, LOW);
//           delay(250);
//           digitalWrite(LED_PIN, HIGH);
//         }
//       }

//       //we exit the "menu"so we can turn the MIDI thru back on
//       MIDI.turnThruOn();
//     }
//   }
// }




// //return 0 if not inverted, not 0 otherwise
uint64_t isInverted(uint8_t index) {
  uint64_t offset = (uint64_t)index;
  return (activePreset.invertBits & ((uint64_t)1 << offset)) >> offset;
}

// void clearBits64(uint64_t & value, uint8_t index) {
//   value = value & ~(((uint64_t)1) << index);
// }
