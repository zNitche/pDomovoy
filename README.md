# pDomovoy

pDomovoy (pD) - room intrusion detection system

### Description
Some time ago I built a [sleepwalking detection and prevention system](https://github.com/zNitche/sleepwalker). It served its purpose good but extensive setup (smartwatch + raspberry Pi 4 + wiring) and the fact that in previous years I've traveled a lot and stayed at many different hotels made me rethink that design.

Then this idea came to my mind, dedicated devices, with long lasting battery life, travel safe (no strong magnets, detachable battery), that serves as `sleepwalker` did but also provides a way to make sure your room's doors stays closed when you sleep (in short nobody enters and leaves without triggering the alarm).

So let's build 2 devices system connected via BLE using C and `pico-sdk`.

### Modules breakdown

#### Warden

##### Description
Heavy lifting device, after initialization (by button push) placed against the door, if device's position / orientation changes, the alarm will be activated.

Triggered alarm can by disabled by:
- pushing button 5 times
- using Trumpet's control panel

##### Assembly
Setup is quite simple, but due to lack of my PCB production capacities, everything had to be assembled using prototype boards + point to point wiring, anyway it looks like this:

- 2x 30x70 mm prototype 2-side boards, some wires, socket for glass tube fuse and cell holder for 18650 battery

- TP4056 (with DW01a for cell protection) for 18650 LI-ION charging (connected via 1A fuse)
- LED and buzzer connected via BC337 transistor for 5V supply and 3.3V control line, connected via 440R resistor to Raspberry Pi Pico W (probably should add 3.3V zener diode to logic line as a extra protection, but due to reasons mentioned earlier I did not)
- Button connected via 440R resistor to MCU
- 2k2 + 2k2 resistors as voltage divider (with ratio 1/2) connected to battery and MCU ADC as voltmeter used for checking battery level.
- ADXL345 accelerometer connected to RPI Pico's 3.3V line and I2C 1
- MCU powered via 5V step-up DC converter
- 220uF capacitors added to Pico's 5V input & step-up converter's input
- 1uF capacior added in parrel to the buzzer for eliminating voltage spikes while generating tone

- 3D printed and mounted using 4x 2mm and 4x 3mm screws
- 2x 2mm screws for case closing

##### Photos
![warden_case_assembled_1](resources/media/placeholder)
![warden_case_closed_1](resources/media/placeholder)

#### Trumped
TODO

### Setup

1. get pico-sdk
```
mkdir -p ~/SDKs/pico-sdk
cd ~/SDKs/pico-sdk
git clone -b "2.2.0" --single-branch https://github.com/raspberrypi/pico-sdk.git 2.2.0
```

2. initialize pico-sdk dependencies
```
cd ~/SDKs/pico-sdk/2.2.0
git submodule update --init
```

3. initialize project dependencies
```
git submodule update --init
```

4. open project in devcontainer
5. setup project

```
cmake -S . -B build
```

or for debug mode

```
cmake -DDEBUG=1 -S . -B build
```

6. build
```
cd build
make
```

7. flash device with generated `.uf2` file (`trumpet` / `warden`)

### Extra

#### auto copying of `.uf2` file to MCU.
```
sh copy_uf2.sh ./build/warden.uf2 /Volumes/RPI-RP2
```

#### Connect to device
```
screen /dev/tty1
```

to exit 
```
CTRL + A -> CTRL + \
```

### Dependencies
- [pico-sdk](https://github.com/raspberrypi/pico-sdk)

- [ADXL345](https://github.com/zNitche/pico-c-adxl345)
- [SSD1306](https://github.com/zNitche/pico-c-ssd1306)
- [AHT20](https://github.com/zNitche/pico-c-aht20)
