# Pi Pico Core1262-868M HF Sample Code

This is some experimantal work to see what can be achieved with [RadioLib](https://github.com/jgromes/RadioLib) on a Pico Pi and a [Waveshare Core1262-868M HF](https://www.waveshare.com/wiki/Core1262-868M) radio module.

<img src="docs/breadboard_radios.jpg" width="400"/>
 
## Wiring
The Pi Pico communicates with the Core1262 over SPI, and with a some extra GPIO pins to for flow control and antenna TX/RX.

The sample code link the Pico and Radio using SPI 0

| Pico GPIO | Pico SPI0 | Core1262 |
|-----------|-----------|----------|
| GPIO2     | SPI0 CLK  | CLK      |
| GPIO4     | SPI0 RX   | MOSO     |
| GPIO3     | SPI0 TX   | MOSI     |
| GPIO5     | SPI0 CSn  | CS       |

## Notes
I'm still not sure if I should be using LDO or DC-DC for voltage regulation. With LDO enabled I can transmit and receive but that does not mean it is the best setup

## Pico circuit diagram
<img src="https://www.raspberrypi.com/documentation/microcontrollers/images/pico-2-r4-pinout.svg" />

## Module circuit diagram
https://files.waveshare.com/upload/c/c1/CoreSX1262_Sch.pdf

I think DC-DC is set up (?)


## References
https://www.waveshare.com/wiki/Core1262-868M<br/>
https://github.com/jgromes/RadioLib/discussions/1556<br/>
https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf<br/>
https://www.waveshare.com/wiki/Core1262-868M<br/>
https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf<br/>
https://github.com/jgromes/RadioLib/discussions/1135<br/>
https://github.com/MarusGradinaru/LoRa-Test/blob/main/src/main.cpp<br/>
