/*
  RadioLib Raspberry Pi Pico example for Waveshare Core1262-868M-hf LoRa module

  Licensed under the MIT License

  Copyright (c) 2024 fruit-bat

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <pico/stdlib.h>

#include "setup.h"

// create a new instance of the HAL class
PicoHal* hal = new PicoHal(
  SPI_PORT,
  SPI_RX, 
  SPI_TX, 
  SPI_SCK
);

SX1262 radio = new Module(
  hal, 
  LORA_CS, 
  LORA_DIO1, 
  LORA_RST, 
  LORA_BUSY
);

uint16_t make_radio() {

  // Just wait a bit before using the serial output, to avoid garbled text
  hal->delay(2000);

  printf("[SX1262] Initializing ... \n");

  // Configure the pins that are used for switching between RX and TX modes.
  // These pins control the RF antenna switch on the Core1262-868M-hf LoRa module.
  // It contains an RTC6603SP SPDT antenna switch
  radio.setRfSwitchPins(LORA_RXEN, LORA_TXEN);

//  radio.invertIQ(true);   // start false, then try true
//  radio.setDio1Action(IrqDio1);

  int16_t state = radio.begin(
    867.2, // frequency in MHz
    125.0, // bandwidth in kHz
    8,     // spreading factor, 7-12 for LoRa
    5,     // coding rate denominator, 4-8 (4 means no coding)
    0x12,  // sync word, 0x12 for private LoRa networks, 0x34 for public LoRa networks
    7,     // output power in dBm, -9 to +22
    18,    // preamble length in symbols
    1.7,   // TCXO voltage
    true   // Use LDO regulator instead of DC-DC (SX1262 only)
  );

  if (state != RADIOLIB_ERR_NONE) {
    printf("failed begin, code %d\n", state);
    return 1;
  }

  hal->delay(10);  // after setTCXO, wait for the TCXO to stabilize before doing anything else with the radio

  printf("Initialise success!\n");

  return 0;
}

