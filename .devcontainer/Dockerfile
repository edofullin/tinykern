FROM ubuntu:24.04

RUN apt update && apt install -y git make cmake qemu-system-misc wget xz-utils python3-dev sudo
RUN usermod -aG sudo ubuntu

# COPY toolchain.tar.xz /tmp/toolchain.tar.xz
# RUN tar -xvf /tmp/toolchain.tar.xz -C /opt && \
#     rm /tmp/toolchain.tar.xz

RUN wget -O /tmp/toolchain.tar.xz https://github.com/riscv-collab/riscv-gnu-toolchain/releases/download/2025.05.30/riscv64-musl-ubuntu-24.04-gcc-nightly-2025.05.30-nightly.tar.xz && \
    tar -xvf /tmp/toolchain.tar.xz -C /opt && \
    rm /tmp/toolchain.tar.xz

ENV PATH="$PATH:/opt/riscv/bin"

