# pDomovoy

pDomovoy (pD) - room intrusion detection system

### Description
Some time ago I built a [sleepwalking detection and prevention system](https://github.com/zNitche/sleepwalker). It served its purpose good but extensive setup (smartwatch + raspberry Pi 4) and the fact that in previous years I've traveled a lot and stayed at many different hotels made me rethink that design.

Then this idea came to my mind, dedicated devices, with long lasting battery life, travel safe (no strong magnets, detachable battery), that serves as `sleepwalker` did but also provides a way to make sure your room's doors stays closed when you sleep (in short nobody enters and leaves without triggering the alarm).

So let's build 2 devices system connected via BLE using C and `pico-sdk`.

### Modules breakdown

#### Warden
TODO

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
