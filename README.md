```
cd pico-sdk
git submodule update --init
```

```
docker compose build --no-cache
docker compose run --rm pico_sdk_worker
```

```
cd /home/build
cmake ..
make -j 8
```

```
screen /dev/tty.1
CTRL + A & CTRL + \
```

