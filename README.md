pDomovoy (pD) - guardian of the sleep


```
mkdir -p ~/SDKs/pico-sdk
cd ~/SDKs/pico-sdk
git clone -b "2.2.0" --single-branch https://github.com/raspberrypi/pico-sdk.git 2.2.0
```

```
cd ~/SDKs/pico-sdk/2.2.0
git submodule update --init
```

Open devcontainer

```
cmake -DDEBUG=1 -S . -B build

cd build
make -j 16
```

```
screen /dev/tty.1
CTRL + A & CTRL + \
```
