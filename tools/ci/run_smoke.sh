#!/usr/bin/env bash
set -euo pipefail
build/melonDS --romless --benchmark 60 > /dev/null 2>&1
