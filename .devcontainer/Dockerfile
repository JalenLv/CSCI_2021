FROM ubuntu:22.04

RUN yes | unminimize

RUN apt update && apt install -y \
    build-essential \
    curl \
    file \
    gdb \
    git \
    man \
    psmisc \
    python3 \
    screen \
    valgrind \
    wget \
    zip

# Set container timezone and locale
RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -y \
    locales \
    tzdata
ARG TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && locale-gen

# Install Rust
RUN curl --proto '=https' --tlsv1.2 https://sh.rustup.rs -sSf | sh -s -- -y --profile minimal

# Need to set up x86-64 virtualization if running on an ARM host
ARG TARGETARCH
RUN if [ "$TARGETARCH" = "arm64" ]; then apt update && \
    apt install -y gdb-multiarch qemu-user gcc-x86-64-linux-gnu; fi
RUN if [ "$TARGETARCH" = "arm64" ]; then mkdir /etc/qemu-binfmt; fi
RUN if [ "$TARGETARCH" = "arm64" ]; then ln -s /usr/x86_64-linux-gnu /etc/qemu-binfmt/x86_64; fi

# Ubuntu 22.04 default clang-format package is too old
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
RUN echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main" >> \
    /etc/apt/sources.list
RUN echo "deb-src http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main" >> \
    /etc/apt/sources.list
RUN apt update && apt install -y clang-format-19
RUN ln -s /usr/bin/clang-format-19 /usr/bin/clang-format
