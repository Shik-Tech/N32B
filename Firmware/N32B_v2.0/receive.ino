// //Is executed everytime a Sysex message is received
// void sysExInterpreter(byte* data, unsigned messageLength) {

//   //check if this is a valid Sysex message
//   if (data[MANUFACTURER] == BASTL_MANUFACTURER_ID) {
//     //check the command byte and acts accordingly
//     switch (data[COMMAND]) {

//       case SETKNOBASGLOBALCC :  //Sets a knob as a global CC knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           //PARAM 2 : the CC number
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC = data[PARAM2];
//             activePreset.knobInfo[knobIndex].NRPN = 0;
//             activePreset.knobInfo[knobIndex].SYSEX = 128;

//             //knob in normal mode by default
//             clearBits64(activePreset.invertBits, data[PARAM1]);
//           }

//           break;
//         }

//       case SETKNOBASINDEPCC :   //Sets a knob as an independent CC knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           //PARAM 2 : CC number
//           //PARAM 3 : the MIDI channel of that knob
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC = data[PARAM2];
//             activePreset.knobInfo[knobIndex].NRPN = 0;
//             activePreset.knobInfo[knobIndex].SYSEX = data[PARAM3] | 0x80;

//             //knob in normal mode by default
//             clearBits64(activePreset.invertBits, data[PARAM1]);
//           }

//           break;
//         }

//       case DISABLEKNOB :  //Sets a knob as an inactive CC knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC  = 0;  //bullshit CC
//             activePreset.knobInfo[knobIndex].NRPN = 0;
//             activePreset.knobInfo[knobIndex].SYSEX = 17 | 0x80; //out of range -> knob disabled
//           }

//           break;
//         }

//       case SETKNOBASBNRPN : //Sets a knob as a Bipolar NRPN (-63, +63) knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           //PARAM 2 : NRPN number LSB
//           //PARAM 3 : NRPN number MSB
//           //PARAM 4 : NRPN range (-range to +range), range max : 63
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t range = data[PARAM4];
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC = data[PARAM2];
//             activePreset.knobInfo[knobIndex].NRPN = data[PARAM3] | 0x80;
//             if (range > 63) range = 63;
//             activePreset.knobInfo[knobIndex].SYSEX = 128 + range;

//             //knob in normal mode by default
//             clearBits64(activePreset.invertBits, data[PARAM1]);
//           }

//           break;
//         }

//       case SETKNOBASUNRPN : //Sets a knob as a Unipolar NRPN (0, +127) knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           //PARAM 2 : NRPN number LSB
//           //PARAM 3 : NRPN number MSB
//           //PARAM 4 : NRPN range (0 to +range), max range : 127
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC = data[PARAM2] | 0x80;
//             activePreset.knobInfo[knobIndex].NRPN = data[PARAM3] | 0x80;
//             activePreset.knobInfo[knobIndex].SYSEX = 128 + data[PARAM4];

//             //knob in normal mode by default
//             clearBits64(activePreset.invertBits, data[PARAM1]);
//           }

//           break;
//         }


//       //for the ranges +164, +200, +1600 and +2000
//       case SETKNOBASENRPN : //Sets a knob as an Extended Unipolar NRPN (0, XX) knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           //PARAM 2 : NRPN number LSB
//           //PARAM 3 : NRPN number MSB
//           //PARAM 4 : NRPN range (1 : +164, 1 : +200, 1 : +1600, 1 : +2000)
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t range = data[PARAM4];
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC = data[PARAM2];
//             activePreset.knobInfo[knobIndex].NRPN = data[PARAM3] | 0x80;
//             switch (range) {
//               case 1 :
//                 range = 63 + 1; //+164
//                 break;
//               case 2 :
//                 range = 63 + 2; //+200
//                 break;
//               case 3 :
//                 range = 63 + 3; //+1600
//                 break;
//               case 4 :
//                 range = 63 + 4; //+2000
//                 break;
//               default :
//                 range = 63 + 10; //wrong number -> no action
//                 break;
//             }
//             activePreset.knobInfo[knobIndex].SYSEX = 128 + range;

//             //knob in normal mode by default
//             clearBits64(activePreset.invertBits, data[PARAM1]);
//           }

//           break;
//         }

//       case SETKNOBASDX :  //Sets a knob as a DX7 parameter change knob
//         {
//           //PARAM 1 : which knob do we affect ?
//           //PARAM 2 : DX7 parameter number most significant bit
//           //PARAM 3 : DX7 parameter number last 7 bits
//           //PARAM 4 : maximum value that can be reached by that parameter
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             uint8_t knobIndex = data[PARAM1];
//             activePreset.knobInfo[knobIndex].CC = 0;
//             activePreset.knobInfo[knobIndex].NRPN = (data[PARAM2] << 7) | data[PARAM3];
//             activePreset.knobInfo[knobIndex].SYSEX = data[PARAM4];

//             //knob in normal mode by default
//             clearBits64(activePreset.invertBits, data[PARAM1]);
//           }

//           break;
//         }

//       case INVERTKNOB : //Sets a knob to be inverted or not
//         {
//           //PARAM1 : which knob do we affect ?
//           //PARAM2 : 0-> knob in normal mode; 1-> knob in invert mode
//           if (data[PARAM1] < NUMBEROFKNOBS) {
//             if (data[PARAM2] == 0) {
//               clearBits64(activePreset.invertBits, data[PARAM1]);
//             }
//             else {
//               activePreset.invertBits |=  (((uint64_t)1) << data[PARAM1]);
//               //bitSet(invertBits, data[PARAM1]);
//             }
//           }
//           break;
//         }


//       case DROPNRPNLSB :
//         {
//           if(data[PARAM1] > 0) {
//             activePreset.dropNRPNLSBvalue = true;        
//           } else {
//             activePreset.dropNRPNLSBvalue = false;
//           }
//         }

//       case PRESETSAVE : //Saves the current state of the machine to the specified slot
//         {
//           //PARAM 1 : the slot where the preset will be saved

//           if (data[PARAM1] < 5) {
//             savePreset(data[PARAM1]);
//           }
//           break;
//         }

//       case PRESETLOAD : //Sets the state of the machine according to the specified preset
//         {
//           //PARAM 1 : number of the preset to load

//           if (data[PARAM1] < 5) {
//             loadPreset(data[PARAM1]);
//           }

//           break;
//         }

//       case SYNCKNOBS :  //Forces the emission of the messages associated to every knob
//         {
//           //NO PARAM
//           for (uint8_t i = 0; i < NUMBEROFKNOBS; i++) {
//             interpretKnob(i, true, false);
//           }

//           break;
//         }

//       case CHANNELCHANGE :  //Changes the global MIDI channel according to the one specified
//         {
//           //PARAM 1 : MIDI channel  (1 to 16)
//           if (data[PARAM1] < 17 && data[PARAM1] > 0) {
//             activePreset.channel = data[PARAM1];
//           }

//           break;
//         }

//       case RANDOMIZER : //Sends random CC messages forthe CC range specified
//         {
//           //PARAM 1 : Min CC number
//           //PARAM 2 : Max CC number
//           if (data[PARAM1] < 127 && data[PARAM2] >= data[PARAM1]) {
//             for (int i = data[PARAM1]; i <= data[PARAM2]; i++) {
//               MIDI.sendControlChange(i, random(0, 127), activePreset.channel);
//             }

//           }
//           break;
//         }
//     }
//   }
// }

// //Is executed everytime a MIDI Program Change message is received
// //loads the specified preset
// void handleProgramChange(byte channel, byte number) {
//   if (number < 5) {
//     loadPreset(number);
//   }
// }

