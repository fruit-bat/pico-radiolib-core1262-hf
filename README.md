# Pi Pico Core1262-868M HF Sample Code

This is some experimantal work to see what can be achieved with [RadioLib](https://github.com/jgromes/RadioLib) on a Pico Pi and a [Waveshare Core1262-868M HF](https://www.waveshare.com/wiki/Core1262-868M) radio module.

The project builds using the [PicoSDK](https://github.com/raspberrypi/pico-sdk).

<img src="docs/breadboard_radios.jpg" width="400"/>
 
## Core1262 Module
The [Core1262 module](https://www.waveshare.com/wiki/Core1262-868M) contains:
* An [SX1262 radio module](https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf)
* A temperature compensated crystal oscillator (TCXO)
* An [RTC6603SP SPDT antenna switch](https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf)
* Some RF conditioning circuitry

I *think*:
* SX1262 DIO1 is routed to the DIO1 module pin 
* SX1262 DIO2 is routed to the DIO2 module pin 
* SX1262 DIO3 is used to enable the TCXO (it is connected to the Vcc pin via a ferrite).

The [RTC6603SP SPDT antenna switch](https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf) has
two control lines (RXEN and TXEN) which are exposed on the module. According to the datasheet, they are used as follows:

| RXEN | TXEN | Operation |
|------|------|-----------|
| H    | L    | Receive   |
| L    | H    | Transmit  |

Unfortunetly, this means we need two GPIO pins to control the antenna direction (correct me if I am wrong here). 

## Wiring
The Pi Pico communicates with the Core1262 over SPI, and some extra GPIO pins to for flow control and antenna TX/RX.

The sample code links the Pico and Radio module on SPI 0 as follows:

| Pico GPIO | Pico SPI0 | Core1262 | SX1262      |
|-----------|-----------|----------|-------------|
| GPIO2     | SPI0 CLK  | CLK      | SX1262 CLK  |
| GPIO3     | SPI0 TX   | MOSI     | SX1262 MOSI |
| GPIO4     | SPI0 RX   | MOSO     | SX1262 MOSO |
| GPIO5     | SPI0 CSn  | CS       | SX1262 CS   |

The remaining SX1262 connections are as follows:

| Pico GPIO | Core1262 | SX1262       |
|-----------|----------|--------------|
| GPIO6     | RESET    | SX1262 RESET |
| GPIO7     | BUSY     | SX1262 BUSY  |
| GPIO20    | DIO1     | SX1262 DIO1  |

Antenna control:

| Pico GPIO | Core1262 | RTC6603SP    |
|-----------|----------|--------------|
| GPIO21    | TXEN     | V2           |
| GPIO22    | RXEN     | V1           |

## Build
The RadioLib and this repository need to be fetched to a common parent folder:
```sh
git clone git@github.com:jgromes/RadioLib.git
git clone git@github.com:fruit-bat/pico-radiolib-core1262-hf.git
cd pico-radiolib-core1262-hf
```

There are a couple of script to help build the project:
```sh
./clean.sh
./build.sh
```

Note that, ```build.sh``` is currently configured for a Pico2 board.

If the build succeeds, it will create two executables, one configured to send and one set to receive:
| Name | Action |
|------|--------|
| pico-sx1262-hx-rx.uf2 | Continuously listen for incomming messages |
| pico-sx1262-hx-tx.uf2 | Periodically transmit a message |

These need to be copied onto the Pi Pico boards.
On the receiver:
```sh
cp pico-sx1262-hx-rx.uf2 /media/neo/RP2350/
```
On the transmitter:
```sh
cp pico-sx1262-hx-tx.uf2 /media/neo/RP2350/
```

## Notes
I'm still not sure if I should be using LDO or DC-DC for voltage regulation. With LDO enabled I can transmit and receive but that does not mean it is the best setup

## Pico circuit diagram
<img src="https://www.raspberrypi.com/documentation/microcontrollers/images/pico-2-r4-pinout.svg" />

## Module circuit diagram
https://files.waveshare.com/upload/c/c1/CoreSX1262_Sch.pdf

From the circuit diagram, I think DC-DC should also work. I might investigate this a bit more.


## References
https://www.waveshare.com/wiki/Core1262-868M<br/>
https://github.com/jgromes/RadioLib/discussions/1556<br/>
https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf<br/>
https://www.waveshare.com/wiki/Core1262-868M<br/>
https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf<br/>
https://github.com/jgromes/RadioLib/discussions/1135<br/>
https://github.com/MarusGradinaru/LoRa-Test/blob/main/src/main.cpp<br/>
