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

// https://github.com/jgromes/RadioLib/discussions/1556
// https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf
// https://www.waveshare.com/wiki/Core1262-868M?srsltid=AfmBOorHuwINCqix4I2qYEq3tzIeVDUvnDJwiwWjryIrZAl860_QI15z
// https://www.waveshare.com/wiki/Core1262-868M
// https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf
// https://github.com/jgromes/RadioLib/discussions/1135
// https://github.com/MarusGradinaru/LoRa-Test/blob/main/src/main.cpp

#include <pico/stdlib.h>

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "hal/RPiPico/PicoHal.h"

// Setup for Waveshare Core1262 HF LoRa Module, S
// X1262 Chip, Long-Range Communication, 
// Anti-Interference, Suitable For Sub-GHz Band, 
// Suitable for 850~930MHz Frequency Band
// Attempt to build for Pico2 

// SPI definition
#define LORA_SCK     2
#define LORA_MISO    4
#define LORA_MOSI    3
#define LORA_CS      5
#define LORA_RST     6
#define LORA_DIO1    20
#define LORA_BUSY    7
#define LORA_TXEN    21
#define LORA_RXEN    22

// define pins to be used
#define SPI_PORT spi0
#define SPI_RX LORA_MISO
#define SPI_TX LORA_MOSI
#define SPI_SCK LORA_SCK

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

int main() {

  stdio_init_all();

  gpio_init(LORA_RXEN);
  gpio_set_dir(LORA_RXEN, GPIO_OUT);

  gpio_init(LORA_TXEN);
  gpio_set_dir(LORA_TXEN, GPIO_OUT);

  // start in RX-safe state
  gpio_put(LORA_RXEN, 1);
  gpio_put(LORA_TXEN, 0);

  // wait for a second before transmitting again
  hal->delay(2000);

  // initialize just like with Arduino
  printf("[SX1262] Initializing ... \n");


  radio.setRfSwitchPins(LORA_RXEN, LORA_TXEN);
//radio.invertIQ(true);   // start false, then try true
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
    return(1);
  }

  hal->delay(10);  // after setTCXO, wait for the TCXO to stabilize before doing anything else with the radio

  printf("Initialise success!\n");

  while(false) {

    printf("[SX1262] Waiting for incoming transmission ... ");

    
    uint8_t buf[255];
    state = radio.receive(buf, sizeof(buf), 10000);

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      printf("%d success!\n", state);

      // print the data of the packet
     // printf("[SX1262] Data:\t\t %s\n", byteArr);
      printf("[SX1262] HEX: ");
      for(size_t i = 0; i < sizeof(buf); i++) {
        printf("%02X ", buf[i]);
      }
      printf("\n[SX1262] ASCII: ");
      for(size_t i = 0; i < sizeof(buf); i++) {
        if(buf[i] >= 32 && buf[i] <= 126) {
          printf("%c", buf[i]);
        } else {
          printf(".");
        }
      }
      printf("\n");


      // print the RSSI (Received Signal Strength Indicator)
      // of the last received packet
      printf("[SX1262] RSSI:\t\t%f dBm\n", radio.getRSSI());

      // print the SNR (Signal-to-Noise Ratio)
      // of the last received packet
      printf("[SX1262] SNR:\t\t%f dB\n", radio.getSNR());

      // print frequency error
      printf("[SX1262] Frequency error:\t%f Hz\n", radio.getFrequencyError());

    } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
      // timeout occurred while waiting for a packet
      printf("timeout!\n");

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      printf("CRC error!\n");

    } else {
      // some other error occurred
      printf("failed, code %d\n", state);

    }
  }






  // loop forever
  for(;;) {
    // send a packet
    printf("[SX1262] Transmitting packet ... ");
    uint8_t data[255] = "Hello World 1234567890";
    state = radio.transmit(data, 12);
    if(state == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      printf("success!\n");


    } else {
      printf("failed, code %d\n", state);

    }
      // wait for a second before transmitting again
      hal->delay(10000);

  }

  return(0);
}
