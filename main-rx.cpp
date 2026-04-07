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

// Setup for Waveshare Core1262 HF LoRa Module
// X1262 Chip, Long-Range Communication, 
// Suitable for 850~930MHz Frequency Band

int main() {

  stdio_init_all();

  if (make_radio() != 0) {
    printf("Failed to initialize radio\n");
    return(1);
  }

  while(true) {

    printf("[SX1262] Waiting for incoming transmission ... ");
    
    uint8_t buf[255];
    int16_t state = radio.receive(buf, sizeof(buf), 10000);

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

  return(0);
}
