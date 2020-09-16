/*
    N32B Firmware v2.0
    MIT License

    Copyright (c) 2020 SHIK
*/

#ifndef N32B_DISPLAY_h
#define N32B_DISPLAY_h

class N32B_DISPLAY
{
public:
    N32B_DISPLAY() {}
    void displayAnimation(DigitLedDisplay display, uint8_t value)
    {
        display.clear();
        uint8_t mappedValue = map(value, 0, 127, 0, 9);

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
        channelDisplayTimeElapsed = 0;
    }
    void showCurrentChannel(DigitLedDisplay display, uint8_t currentChannel)
    {
        if (channelDisplayTimeElapsed >= channelDisplayInterval * 1000)
        {
            display.clear();
            display.printDigit(currentChannel);
        }
    }

public:
private:
    elapsedMillis channelDisplayTimeElapsed;
    uint8_t channelDisplayInterval = 1;
};

#endif
