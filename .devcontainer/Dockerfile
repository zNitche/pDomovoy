FROM ubuntu:24.04

RUN apt update && apt install -y git cmake python3 build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib

RUN mkdir /home/pico_sdk_worker

ENV PICOTOOL_FETCH_FROM_GIT_PATH=/home/pico_sdk_worker/build
ENV PICO_SDK_PATH=/home/SDKs/pico-sdk/2.2.0
