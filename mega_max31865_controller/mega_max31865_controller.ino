#include <Adafruit_MAX31865.h> // Library to operate the MAX31865.
#include <pt100rtd.h> // Library with a PT100 look up table.

const uint8_t CIPO = 50; // SPI - Controller in peripheral out.
const uint8_t COPI = 51; // SPI - Controller out peripheral in.
const uint8_t CLK = 52; // SPI - Clock.
const uint8_t pt100_blank = 255; // This is just a blank chip select used to push the array forward one.
const uint8_t pt100_1 = 5; // SPI - Chip select pin.
const uint8_t pt100_2 = 6; // SPI - Chip select pin.
const uint8_t pt100_3 = 7; // SPI - Chip select pin.
const uint8_t pt100_4 = 8; // SPI - Chip select pin.

const uint32_t baud = 115200; // Serial - Communications speed.
const char start_marker = '<'; // Character to initiate building received characters.
const char end_marker = '>'; // Character to signify end of received characters.

uint8_t sensor = 0; // Holds the current sensor being used. Set in "check_serial_port".
float acquisitions = 0; // Determines the number of acquisitions to be averaged over. Set in "check_serial_port".

boolean new_command = false; // Holds weather a new command has been received. Set in "check_serial_port".
const byte char_length = 10; // Used to set the size of the received character array.
char received_chars[char_length]; // Array used to hold the received characters. Filled in "check_serial_port".
char temporary_chars[char_length]; // Array used to copy the original

#define RREF 430.0 // The value of the Rref resistor.
#define RNOMINAL 100.0 // Resistance of the PT100 at 0°C.

// Creating an object array of attached MAX31865 devices.
// This should be increased or decreased depending on the number of connected boards. Adda a new chip select for each new device.
Adafruit_MAX31865 sensors[] = {
  Adafruit_MAX31865(pt100_blank, COPI, CIPO, CLK),
  Adafruit_MAX31865(pt100_1, COPI, CIPO, CLK),
  Adafruit_MAX31865(pt100_2, COPI, CIPO, CLK),
  Adafruit_MAX31865(pt100_3, COPI, CIPO, CLK),
  Adafruit_MAX31865(pt100_4, COPI, CIPO, CLK)
};

// Creating an object array of Serial ports.
// This can be extended where necessary. The was added so the RS232 board as well as the USB port can be used for communication.
HardwareSerial *serial_ports[] = {&Serial, &Serial3};

const uint8_t connected_ports = sizeof(serial_ports) / sizeof(serial_ports[0]); // Determining the number of communication ports.
const uint8_t connected_boards = sizeof(sensors) / sizeof(sensors[0]); // Determining the number of connected MAX31865 boards.

pt100rtd PT100 = pt100rtd(); // Initializing the PT100 look up library.

void setup() {
  // Initializing the serial ports. All ports are using the same baud.
  for (uint8_t i = 0; i < connected_ports; i++) {
    serial_ports[i]->begin(baud);
  }
  // Initializing the MAX31865 boards. All boards are using 4 wire configuration.
  for (uint8_t i = 0; i < connected_ports; i++) {
    sensors[i].begin(MAX31865_4WIRE);
  }
}

void loop() {
  //For each connected serial port, as determined above.
  for (uint8_t i = 0; i < connected_ports; i++) {
    // Check the serial port for activity.
    check_serial_port(serial_ports[i]);
    // Execute any commands that came over the serial line.
    run_new_command(serial_ports[i]);
  }
}
