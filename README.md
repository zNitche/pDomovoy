```
mkdir -p ~/SDK/pico-sdk
cd ~/SDK/pico-sdk
git clone -b "2.2.0" --single-branch https://github.com/raspberrypi/pico-sdk.git 2.2.0
```

```
cd ~/SDK/pico-sdk/2.2.0
git submodule update --init
```

Open devcontainer

```
cd /home/pico_sdk_worker
cmake -S . -B build

or

cmake -DPICO_BOARD=pico_w -S . -B build

cd build
make -j 16
```

```
screen /dev/tty.1
CTRL + A & CTRL + \
```
