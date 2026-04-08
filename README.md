# Pi Pico Core1262-868M HF Sample Code

This is some experimental work to see what can be achieved with [RadioLib](https://github.com/jgromes/RadioLib) on a Pico Pi wired to a [Waveshare Core1262-868M HF](https://www.waveshare.com/wiki/Core1262-868M) radio module.

The objective is a minimal setup to demonstrate transmission and reception.

The project builds using the [PicoSDK](https://github.com/raspberrypi/pico-sdk).

If you can see mistakes or are able to suggest improvements please raise an issue; it would be nice to if this repository was useful to people. 

<img src="docs/breadboard_radios.jpg" width="400"/>
 
## Core1262 Module
The [Core1262 module](https://www.waveshare.com/wiki/Core1262-868M) contains:
* An [SX1262 radio module](https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf)
* A temperature compensated crystal oscillator (TCXO)
* An [RTC6603SP SPDT antenna switch](https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf)
* Some RF conditioning circuitry

I *believe*:
* SX1262 DIO1 is routed solely to the DIO1 module pin 
* SX1262 DIO2 is routed solely to the DIO2 module pin 
* SX1262 DIO3 is used to enable the TCXO (it is connected to the Vcc pin via a ferrite).

The [RTC6603SP SPDT antenna switch](https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf) has
two control lines (RXEN and TXEN) which are exposed on the module. According to the datasheet, they are used as follows:

| RXEN | TXEN | Operation |
|------|------|-----------|
| H    | L    | Receive   |
| L    | H    | Transmit  |

Unfortunately, this means we need two GPIO pins to control the antenna direction (correct me if I am wrong here). 

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
Both the RadioLib repository and this repository need to be fetched to a common parent folder:
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

If the build succeeds, it will create two executables, one configured to send, and one set to receive:
| Name | Action |
|------|--------|
| pico-sx1262-hx-rx.uf2 | Continuously listen for incoming messages |
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

## Working configuration for the Core1262-868M HF
These are sample settings that are enough to get transmission and reception working:
```cpp
  // Configure the pins that are used for switching between RX and TX modes.
  // These pins control the RF antenna switch on the Core1262-868M-hf LoRa module.
  // It contains an RTC6603SP SPDT antenna switch
  radio.setRfSwitchPins(LORA_RXEN, LORA_TXEN);

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
'''

## Notes
I'm still not sure if I should be using LDO or DC-DC for voltage regulation. With LDO enabled I can transmit and receive but that does not mean it is the best setup.

I'm not sure if the TXCO voltage should be set to 1.7v or 1.8v. I currently have it running at 1.7v

From the circuit diagram, I think DC-DC should as well as LDO. I might investigate this a bit more.

RadioLib does not seem to set the initial state of the antenna swtich pins RXEN and TXEN. This makes me nervous, so I have added a line to ```Module.cpp``` as follows:
```cpp
void Module::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  // This can be on the stack, setRfSwitchTable copies the contents
  const uint32_t pins[] = {
    rxEn, txEn, RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC,
  };
  
  // This must be static, since setRfSwitchTable stores a reference.
  static const RfSwitchMode_t table[] = {
    { MODE_IDLE,  {this->hal->GpioLevelLow,  this->hal->GpioLevelLow} },
    { MODE_RX,    {this->hal->GpioLevelHigh, this->hal->GpioLevelLow} },
    { MODE_TX,    {this->hal->GpioLevelLow,  this->hal->GpioLevelHigh} },
    END_OF_MODE_TABLE,
  };
  setRfSwitchTable(pins, table);
}

```
is now:
```cpp
void Module::setRfSwitchPins(uint32_t rxEn, uint32_t txEn) {
  // This can be on the stack, setRfSwitchTable copies the contents
  const uint32_t pins[] = {
    rxEn, txEn, RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC,
  };
  
  // This must be static, since setRfSwitchTable stores a reference.
  static const RfSwitchMode_t table[] = {
    { MODE_IDLE,  {this->hal->GpioLevelLow,  this->hal->GpioLevelLow} },
    { MODE_RX,    {this->hal->GpioLevelHigh, this->hal->GpioLevelLow} },
    { MODE_TX,    {this->hal->GpioLevelLow,  this->hal->GpioLevelHigh} },
    END_OF_MODE_TABLE,
  };
  setRfSwitchTable(pins, table);
  // set the initial state to RX (some switches do not have an idle state)
  setRfSwitchState(MODE_RX);
}
```
I may just be missing something here, I will try to get round to asking the author.

In RadioLib there does not seem to be a way to initialise the sx1262 without telling it to use DIO2 as antenna control. See ```SX126x.cpp```:
```cpp
int16_t SX126x::begin(uint8_t cr, uint8_t syncWord, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // BW in kHz and SF are required in order to calculate LDRO for setModulationParams
  // set the defaults, this will get overwritten later anyway
  this->bandwidthKhz = 500.0;
  this->spreadingFactor = 9;

...

  // set publicly accessible settings that are not a part of begin method
  state = setCurrentLimit(60.0);
  RADIOLIB_ASSERT(state);

  state = setDio2AsRfSwitch(true);
  RADIOLIB_ASSERT(state);

  state = setCRC(2);
  RADIOLIB_ASSERT(state);

  state = invertIQ(false);
  RADIOLIB_ASSERT(state);

  return(state);
}
```
Now, it's possible this does not matter, but it would be nice for this value, along with the other hard coded ones passed in to begin with defaults. Depending on what external circuitry is added to the module, enabling DIO2 as an output, if only briefly, might cause damage.

The above code also enables CRC. In Lora mode the radio either has CRC enabled or not. Passing 0 to setCRC disables CRC. 

For now I have disabled the use of DIO2 as follows:
```cpp
int16_t SX126x::begin(uint8_t cr, uint8_t syncWord, uint16_t preambleLength, float tcxoVoltage, bool useRegulatorLDO) {
  // BW in kHz and SF are required in order to calculate LDRO for setModulationParams
  // set the defaults, this will get overwritten later anyway
  this->bandwidthKhz = 500.0;
  this->spreadingFactor = 9;

...

  // set publicly accessible settings that are not a part of begin method
  state = setCurrentLimit(60.0);
  RADIOLIB_ASSERT(state);

  state = setDio2AsRfSwitch(false); // Was true. Should be a parameter?
  RADIOLIB_ASSERT(state);

  state = setCRC(2);
  RADIOLIB_ASSERT(state);

  state = invertIQ(false);
  RADIOLIB_ASSERT(state);

  return(state);
}
```


## Pi Pico circuit diagram
<img src="https://www.raspberrypi.com/documentation/microcontrollers/images/pico-2-r4-pinout.svg" />

## Module circuit diagram
https://files.waveshare.com/upload/c/c1/CoreSX1262_Sch.pdf

## References
https://www.waveshare.com/wiki/Core1262-868M<br/>
https://github.com/jgromes/RadioLib/discussions/1556<br/>
https://files.waveshare.com/upload/c/c6/Datasheet-RTC6603SP-RichWave.pdf<br/>
https://www.waveshare.com/wiki/Core1262-868M<br/>
https://files.waveshare.com/upload/e/e1/DS_SX1261-2_V1.2.pdf<br/>
https://github.com/jgromes/RadioLib/discussions/1135<br/>
https://github.com/MarusGradinaru/LoRa-Test/blob/main/src/main.cpp<br/>
