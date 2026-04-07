#pragma once
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

#include <RadioLib.h>

#include "hal/RPiPico/PicoHal.h"

// Pico Pi GPIO allocations for Waveshare Core1262-868M-hf LoRa module
#define LORA_SCK     2
#define LORA_MISO    4
#define LORA_MOSI    3
#define LORA_CS      5
#define LORA_RST     6
#define LORA_DIO1    20
#define LORA_BUSY    7
#define LORA_TXEN    21
#define LORA_RXEN    22

// Pico Pi SPI pins to be used
#define SPI_PORT spi0
#define SPI_RX LORA_MISO
#define SPI_TX LORA_MOSI
#define SPI_SCK LORA_SCK

extern PicoHal* hal;
extern SX1262 radio;    

uint16_t make_radio();
