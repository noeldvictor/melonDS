# .devcontainer/Dockerfile  – also works stand‑alone
FROM ubuntu:24.04

# Build‑time deps
RUN apt-get update && DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        build-essential cmake ninja-build extra-cmake-modules \
        libsdl2-dev libarchive-dev libenet-dev libzstd-dev \
        libcurl4-gnutls-dev libpcap0.8-dev \
        qt6-base-dev qt6-base-private-dev qt6-multimedia-dev libqt6svg6-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace

# Compile so the layer is cached for lint / test cycles
RUN cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo \
 && cmake --build build -j$(nproc)

CMD ["./build/melonDS"]
