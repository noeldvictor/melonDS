# melonDS – AGENTS.md
_Guidelines consumed by automation (OpenAI Codex, GitHub Actions, etc.)._

## 1. Coding conventions
* 4‑space indentation, no tabs.
* Follow the style rules in `CONTRIBUTING.md`; **Codex MUST NOT touch them** :contentReference[oaicite:1]{index=1}.
* Prefer `u8/u16/…` typedefs from `types.h`; standard `int` OK for loop vars.

## 2. Project tour
| Path | One‑liner purpose |
|------|------------------|
| `src/` | All emulator code. **High‑risk** – run tests before committing. |
| `res/` | Windows & macOS resources. Touch only when updating icons. |
| `tools/` | Helper scripts (e.g. mac bundler). |
| `freebios/` | Reference BIOS blobs – **read‑only**. |
| `cmake/` | CMake helpers & toolchains. |

## 3. How to build / test locally
```bash
# Linux & macOS
./scripts/setup.sh       # configure + build
./scripts/run‑smoke.sh   # starts ROM‑less emulator for 3 s, exits 0 on success
