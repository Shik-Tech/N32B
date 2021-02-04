/*
    N32B Firmware v2.0
    MIT License

    Copyright (c) 2021 SHIK
*/

#ifndef N32B_DISPLAY_h
#define N32B_DISPLAY_h

class N32B_DISPLAY
{
public:
    DigitLedDisplay display = DigitLedDisplay(16, 10, 15); // Display setup: (DIN, CS, CLK)

    // Set the display brightness (1-15)
    void setBright(uint8_t brightness)
    {
        display.setBright(brightness);
    }

    // Set digits amount
    void setDigitLimit(uint8_t digitLimit)
    {
        display.setDigitLimit(digitLimit);
    }

    // Render current channel or preset
    void renderDisplay(Preset_t activePreset, uint8_t readInterval = 1500)
    {
        if (millis() - channelDisplayTimeElapsed >= readInterval)
        {
            if (!isPresetMode)
            {
                showCurrentChannel();
            }
            else
            {
                showCurrentPreset();
            }
        }
    }

    // Startup animation (n32b)
    void startupAnimation()
    {
        uint8_t delayTime = 200;
        uint8_t repeats = 1;

        for (int i = 0; i < repeats; i++)
        {
            display.clear();
            delay(delayTime + 200);

            // _n
            display.write(1, B00010101);
            delay(delayTime);
            display.clear();

            // n3
            display.write(1, B01111001);
            display.write(2, B00010101);
            delay(delayTime);
            display.clear();

            // 32
            display.write(1, B01101101);
            display.write(2, B01111001);
            delay(delayTime);
            display.clear();

            // 2b
            display.write(1, B00011111);
            display.write(2, B01101101);
            delay(delayTime);
            display.clear();

            // b_
            display.write(2, B00011111);
            delay(delayTime);
            display.clear();

            delay(delayTime + 500);
        }
    }

    // Display the currnet knob value as a square
    void valueAnimation(uint16_t value)
    {
        display.clear();
        uint8_t mappedValue = map(value, 0, 1023, 0, 9);

        // Full circle animation
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
        channelDisplayTimeElapsed = millis();
    }

    // Show animation after factory reset (infinity animation)
    void factoryResetAnimation()
    {
        uint8_t delayTime = 100;
        uint8_t repeats = 3;

        for (int i = 0; i < repeats; i++)
        {
            display.clear();

            display.write(2, B00010000);
            delay(delayTime);
            display.clear();

            display.write(2, B00011000);
            delay(delayTime);
            display.clear();

            display.write(2, B00011100);
            delay(delayTime);
            display.clear();

            display.write(2, B00001101);
            delay(delayTime);
            display.clear();

            display.write(2, B00000101);
            display.write(1, B00000001);
            delay(delayTime);
            display.clear();

            display.write(2, B00000001);
            display.write(1, B00100001);
            delay(delayTime);
            display.clear();

            display.write(1, B01100001);
            delay(delayTime);
            display.clear();

            display.write(1, B01100010);
            delay(delayTime);
            display.clear();

            display.write(1, B01000010);
            display.write(2, B00010000);
            delay(delayTime);
            display.clear();
        }
    }

    // Show the curernt channel
    void showCurrentChannel()
    {
        display.clear();
        display.printDigit(activePreset.channel);
    }

    // Show the curernt preset
    void showCurrentPreset()
    {
        display.clear();
        display.write(2, B01100111);
        display.printDigit(currentPresetNumber);
        presetDisplayTimeElapsed = millis();
    }

    // Show save message (Sv.)
    void showSaveMessage()
    {
        display.clear();
        display.write(2, B01011011);
        display.write(1, B00011100);
        delay(200);
        display.write(1, B10011100);
        delay(200);
        display.write(1, B00011100);
        delay(200);
        display.write(1, B10011100);
        delay(200);
    }

private:
    unsigned long channelDisplayTimeElapsed;
    unsigned long presetDisplayTimeElapsed;
};

#endif
