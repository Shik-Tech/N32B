/*
  N32B Firmware v3.0
  MIT License

  Copyright (c) 2021 SHIK
*/

void processSysex(byte *data, unsigned size)
{
    if (size > 3 && data[MANUFACTURER] == SHIK_MANUFACTURER_ID)
    {
        switch (data[COMMAND])
        {
        case SET_KNOB_AS_CC:
        {
            setKnobAsCC(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
            break;
        }

        case SET_KNOB_AS_CC_CHANNEL:
        {
            setKnobAsCCWithChannel(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX], data[CHANNEL_INDEX]);
            break;
        }

        case SET_KNOB_AS_NRPN:
        {
            setKnobAsNRPN(data[KNOB_INDEX], data[MSB_INDEX], data[LSB_INDEX]);
            break;
        }

        case DISABLE_KNOB:
        {
            setKnobAsDisabled(data[KNOB_INDEX]);
            break;
        }

        case HIGH_RES_14BIT:
        {
            useHighResolution(data[KNOB_INDEX]);
            break;
        }

        case SAVE_PRESET:
        {
            savePreset(data[KNOB_INDEX]);
            break;
        }

        case LOAD_PRESET:
        {
            loadPreset(data[KNOB_INDEX]);
            break;
        }

        case SYNC_KNOBS:
        {
            for (uint8_t i = 0; i < NUMBER_OF_KNOBS; i++)
            {
                interpretKnob(i, true, false);
            }
            break;
        }

        case CHANGE_CHANNEL:
        {
            handleChangeChannel(data[KNOB_INDEX]);
            break;
        }

        case SEND_CURRENT_CONFIG:
        {
            // sendCurrentConfig();
            break;
        }
        }
    }
    else
    {
        // Serial.println("Manufacturer ID is not matching the N32B");
    }
}

void setKnobAsCC(byte knobIndex, byte MSB, byte LSB)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = MSB;
        activePreset.knobInfo[knobIndex].LSB = LSB;
        activePreset.knobInfo[knobIndex].NRPN = 0;
        activePreset.knobInfo[knobIndex].CHANNEL = 128;
    }
}

void setKnobAsCCWithChannel(byte knobIndex, byte MSB, byte LSB, byte channel)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = MSB;
        activePreset.knobInfo[knobIndex].LSB = LSB;
        activePreset.knobInfo[knobIndex].NRPN = 0;
        activePreset.knobInfo[knobIndex].CHANNEL = channel | 0x80;
    }
}

void setKnobAsDisabled(byte knobIndex)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = 0;
        activePreset.knobInfo[knobIndex].NRPN = 0;
        activePreset.knobInfo[knobIndex].CHANNEL = 17 | 0x80; // Make the knob out of range to disable it
    }
}

void setKnobAsNRPN(byte knobIndex, byte LSB, byte MSB)
{
    if (knobIndex < NUMBER_OF_KNOBS)
    {
        activePreset.knobInfo[knobIndex].MSB = MSB;
        activePreset.knobInfo[knobIndex].LSB = LSB;
        activePreset.knobInfo[knobIndex].NRPN = 1;
        activePreset.knobInfo[knobIndex].CHANNEL = 128;
    }
}

void useHighResolution(bool value)
{
    activePreset.highResolution = value;
}

void handleChangeChannel(byte channel)
{
    if (channel < 17 && channel > 0)
    {
        activePreset.channel = channel;
    }
}

// Change preset on program change
void handleProgramChange(byte channel, byte number)
{
    if (number < 5)
    {
        loadPreset(number);
    }
}