#ifndef MUX74HC4067_h
#define MUX74HC4067_h

class MUX74HC4067
{
public:
    MUX74HC4067() {}
    void init(uint8_t channel1, uint8_t channel2, uint8_t channel3, int8_t channel4)
    {
        channels[0] = channel1;
        channels[1] = channel2;
        channels[2] = channel3;
        channels[3] = channel4;
        for (uint8_t i = 0; i < 4; i++)
        {
            pinMode(channels[i], OUTPUT);
        }
        delay(10);
        timeout = millis();
    }
    void setSignalPin(bool muxIndex, uint8_t pin)
    {
        signalPin[muxIndex] = pin;
        pinMode(pin, INPUT);
    }

    template<typename Callback>
    void update(Callback&& doMidiRead)
    {
        if (millis() - timeout >= 1)
        {
            readMux(doMidiRead);
            timeout = millis();
        }
    }
    // uint16_t read(uint16_t knobIndex)
    // {
    //     if (knobIndex < NUMBER_OF_KNOBS)
    //     {
    //         return values[knobIndex];
    //     }
    //     return 0;
    // }

private:
    uint8_t currentChannel = 0;
    uint8_t channels[4];
    uint8_t signalPin[2];
    unsigned long timeout;

    template<typename Callback>
    void readMux(Callback&& doMidiRead)
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            uint8_t index = (i * 16) + currentChannel;
            if (index < NUMBER_OF_KNOBS)
            {
                doMidiRead();
                for (uint8_t j = 3; j > 0; j--)
                {
                    knobBuffer[j][index] = knobBuffer[j - 1][index];
                }
                knobBuffer[0][index] = analogRead(signalPin[i]);
                doMidiRead();
            }
        }
        currentChannel++;
        if (currentChannel >= NUMBER_OF_KNOBS / 2)
        {
            currentChannel = 0;
        }
        for (uint8_t i = 0; i < 4; i++)
        {
            digitalWrite(channels[i], bitRead(currentChannel, i));
        }
    }
};

#endif