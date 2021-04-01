/*
    N32B Firmware v3.0
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

    // Auto clear the display
    void updateDisplay(uint8_t readInterval = 255) {
        if(millis() - displayOffTimer >= readInterval) {
            display.clear();
        }
    }

    // Blink the decimal points
    void blinkDot(uint8_t dotSide, uint8_t readInterval = 255) {
        if(millis() - displayOffTimer >= readInterval) {
            display.clear();
            display.write(dotSide, B10000000);
        }
    }

    void showChannelNumber(uint8_t channelNumber) {
        display.printDigit(channelNumber);
        displayOffTimer = millis();
    }

    void showPresetNumber(byte presetNumber) {
        display.write(2, B01100111);
        display.printDigit(presetNumber);
        displayOffTimer = millis();
    }

    void showStartUpAnimation() {
        uint8_t delayTime = 175;
        uint8_t repeats = 3;

        for (uint8_t i = 0; i < repeats; i++) {
            display.clear();

            display.write(1, B00001000);
            display.write(2, B00001000);
            delay(delayTime);
            display.clear();

            display.write(1, B00000001);
            display.write(2, B00000001);
            delay(delayTime);
            display.clear();

            display.write(1, B01000000);
            display.write(2, B01000000);
            delay(delayTime);
            display.clear();
        }
    }

    // Show animation after factory reset (infinity sign animation)
    void factoryResetAnimation()
    {
        uint8_t delayTime = 100;
        uint8_t repeats = 3;

        for (uint8_t i = 0; i < repeats; i++)
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

    // Show save message (Sv.)
    void showSaveMessage()
    {
        display.clear();
        display.write(2, B01011011);
        display.write(1, B00011100);
        delay(300);
        display.write(1, B10011100);
        delay(300);
        display.write(1, B00011100);
        delay(300);
        display.write(1, B10011100);
        delay(300);
    }

private:
    unsigned long displayOffTimer;
};

#endif
