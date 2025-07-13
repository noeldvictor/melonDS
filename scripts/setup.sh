#!/usr/bin/env bash
set -euo pipefail
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j"$(nproc)"
