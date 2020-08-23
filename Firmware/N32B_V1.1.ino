#include <Control_Surface.h>

// ----------------------------- User Settings ------------------------------ //
 // ========================================================================== //
  
//  constexpr uint32_t ADC_BITS = 14;
//  constexpr uint8_t ADC_BITS = ADC_RESOLUTION;
  
//  constexpr uint8_t ANALOG_FILTER_SHIFT_FACTOR = 2;
  
//  using ANALOG_FILTER_TYPE = uint16_t;
  
//  constexpr unsigned long BUTTON_DEBOUNCE_TIME = 25; // milliseconds
//  constexpr unsigned long LONG_PRESS_DELAY = 450; // milliseconds
//  constexpr unsigned long LONG_PRESS_REPEAT_DELAY = 200; // milliseconds
//  constexpr unsigned long FILTERED_INPUT_UPDATE_INTERVAL = 1000; // microseconds
//  constexpr static Frequency SPI_MAX_SPEED = 8_MHz;
  
//  #define AH_INDIVIDUAL_BUTTON_INVERT
  
 // ========================================================================== //

// Instantiate a MIDI over USB interface.
// USBMIDI_Interface midi;
USBMIDI_Interface usbmidi;
// HardwareSerialMIDI_Interface serialmidi = {Serial1, MIDI_BAUD};
// BidirectionalMIDI_PipeFactory<2> pipes;

// class CC14Potentiometer : public MIDIFilteredAnalog<ContinuousCCSender14<10>> {
//   public:
//     CC14Potentiometer(pin_t analogPin, MIDIAddress address)
//       : MIDIFilteredAnalog(analogPin, address, {}) {}
// };


// Setting up the banks
const byte numberOfBanks = 16;
Bank<numberOfBanks> bank(1);

// Setting up the display pin
uint8_t cs10 = 10; // pin 10 as first Max7219 CS
MAX7219SevenSegmentDisplay max72_10 = {cs10};

// // Mux1 & Mux2 pin setup
CD74HC4067 mux1 = {8, {2, 3, 4, 5}};
CD74HC4067 mux2 = {9, {2, 3, 4, 5}};

IncrementDecrementSelector<numberOfBanks> selector = {
    bank,       // Bank to manage
    {A3, A2},   // push button pins (increment, decrement)
    Wrap::Wrap, // Wrap around
};

// Mux1 Potentiometers
Bankable::CCPotentiometer potentiometers1[] = {
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(15), {1, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(13), {2, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(8), {3, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(6), {4, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(14), {5, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(11), {6, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(7), {7, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(5), {8, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(12), {9, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(10), {10, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(4), {11, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(3), {12, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(9), {13, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(2), {14, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(1), {15, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux1.pin(0), {16, CHANNEL_1}},
};

// Mux2 Potentiometers
Bankable::CCPotentiometer potentiometers2[] = {
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(15), {17, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(14), {18, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(13), {19, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(12), {20, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(8), {21, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(9), {22, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(10), {23, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(11), {24, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(4), {25, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(5), {26, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(6), {27, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(7), {28, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(0), {29, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(1), {30, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(2), {31, CHANNEL_1}},
    {{bank, BankType::CHANGE_CHANNEL}, mux2.pin(3), {32, CHANNEL_1}},
};

void setup()
{
  // Muxs enable pins to LOW
  // digitalWrite(6, LOW);
  // digitalWrite(7, LOW);
  // Serial.begin(115200);
  // Control_Surface | pipes | usbmidi;
  // Control_Surface | pipes | serialmidi;
  max72_10.begin();
  max72_10.clear();
  Control_Surface.begin();
}

void loop()
{
  Control_Surface.loop();
  max72_10.display(bank.getSelection() + 1, 0, 1);
}


