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

  // loop forever
  for(;;) {
    // send a packet
    printf("[SX1262] Transmitting packet ... ");
    uint8_t* data = (uint8_t *)"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed lorem magna, feugiat eget augue id, ultrices semper lorem. Pellentesque vitae enim mauris. Nam at massa ac urna suscipit iaculis. Cras ultrices magna sit amet est volutpat vestibulum erat curae.";
    int16_t state = radio.transmit(data, 255);
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
