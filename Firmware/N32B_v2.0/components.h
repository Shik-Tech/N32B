// /*
//     N32B Firmware v2.0
//     MIT License

//     Copyright (c) 2020 SHIK

//     MUX74HC4067 is a modified library which originally written by neroroxxx:
//     https://github.com/neroroxxx/RoxMux
// */
// #ifndef MUX74HC4067_h
// #define MUX74HC4067_h

// template <uint8_t _muxCount, uint8_t _muxChannels, uint8_t _muxPins>
// class MUX74HC4067
// {
// public:
//     MUX74HC4067() {}
//     void begin(uint8_t ch1, uint8_t ch2, uint8_t ch3, int8_t ch4)
//     {
//         if (_muxChannels != 4)
//         {
//             delay(1000);
//             Serial.println("channel must be 4");
//             while(1);
//         }
//         else if (_muxChannels == 4 && _muxPins != 16)
//         {
//             delay(1000);
//             Serial.println("_muxPins must be 16");
//             while(1);
//         }
//         channels[0] = ch1;
//         channels[1] = ch2;
//         channels[2] = ch3;
//         channels[3] = ch4;
//         for (uint8_t i = 0; i < _muxChannels; i++)
//         {
//             pinMode(channels[i], OUTPUT);
//         }
//         timeout = 0;
//         delay(10);
//     }
//     // @n: the index of the mux
//     // @pin: the pin on the arduino connected to that mux' signal pin
//     // this method must be called for each mux
//     void setSignalPin(uint8_t n, uint8_t pin)
//     {
//         if (n < _muxCount)
//         {
//             signalPin[n] = pin;
//             pinMode(pin, INPUT);
//         }
//     }
//     // These analog multiplexers require a small delay between pin read
//     // so that their capacitor for each pin is charged/discharged
//     // since we don't want to use an actual delay() we use an elapsedMillis
//     // you can pass a value to this function in your sketch to change the
//     // number of milliseconds between each pin read.
//     void update(uint8_t readInterval = 1)
//     {
//         if (timeout >= readInterval)
//         {
//             readMux();
//             timeout = 0;
//         }
//     }
//     uint16_t read(uint16_t n)
//     {
//         if (n < totalPins)
//         {
//             return values[n];
//         }
//         return 0;
//     }

// private:
//     uint8_t currentChannel = 0;
//     uint8_t channels[_muxChannels];
//     uint8_t signalPin[_muxCount];
//     uint16_t values[_muxCount * _muxPins];
//     elapsedMillis timeout;
//     const uint16_t totalPins = (_muxCount * _muxPins);

//     void readMux()
//     {
//         for (uint8_t i = 0; i < _muxCount; i++)
//         {
//             uint8_t index = (i * _muxPins) + currentChannel;
//             if (index < totalPins)
//             {
//                 values[index] = analogRead(signalPin[i]);
//             }
//         }
//         // go to the next channel
//         currentChannel++;
//         if (currentChannel >= _muxPins)
//         {
//             currentChannel = 0;
//         }
//         // set the channel pins
//         for (uint8_t i = 0; i < _muxChannels; i++)
//         {
//             digitalWrite(channels[i], bitRead(currentChannel, i));
//         }
//     }
// };

// template <uint8_t _muxCount>
// class MUX_FACTORY_74HC4067 : public MUX74HC4067<_muxCount, 4, 16> {};

// #endif