FROM ubuntu:24.04

RUN apt update && apt install -y git make cmake qemu-system-misc wget xz-utils python3-dev sudo
RUN usermod -aG sudo ubuntu

# COPY toolchain.tar.xz /tmp/toolchain.tar.xz
# RUN tar -xvf /tmp/toolchain.tar.xz -C /opt && \
#     rm /tmp/toolchain.tar.xz

RUN wget -O /tmp/toolchain.tar.xz https://github.com/riscv-collab/riscv-gnu-toolchain/releases/download/2024.12.16/riscv64-glibc-ubuntu-24.04-llvm-nightly-2024.12.16-nightly.tar.xz && \
    tar -xvf /tmp/toolchain.tar.xz -C /opt && \
    rm /tmp/toolchain.tar.xz

ENV PATH="$PATH:/opt/riscv/bin"

