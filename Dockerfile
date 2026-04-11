FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=nointeractive

RUN apt-get update && apt-get install -y \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    qemu-system-arm \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /project

CMD ["make", "all"]