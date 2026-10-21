# Chip-8 Emulator
A emulator for the Chip-8 hardware writen in C++ with SDL3 for rendering based on [Cowgod's](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) documentation from 1997

| CHIP-8 Logo | IBM Logo | Opcode Test (Corax+) | Flags Test |
| :---: | :---: | :---: | :---: |
| ![CHIP-8 Logo](docs/images/chip-8_logo.png) | ![IBM Logo](docs/images/IBM_logo.png) | ![Opcode Test](docs/images/opcode_test.png) | ![Flags Test](docs/images/flags_test.png) |
| **Quirks Test** | **Keypad Test** | **Audio Beep Test** | **Rock Paper Scissors** |
| ![Quirks Test](docs/images/quirks_test.png) | ![Keyboard Test](docs/images/keyboard_test.png) | ![Beep Test](docs/images/beep_test.png) | ![Rock Paper Scissors](docs/images/RPS.png) |


## Getting started

### Prerequisites
* **C++ Compiler** supporting C++17 or later (MSVC, GCC, or Clang)
* **CMake** (v3.20 or newer)
* **Git**

### 1. Clone the repository
Clone with `--recursive` because SDL3 is a submodule
```bash
git clone --recursive https://github.com/Anomi132/CHIP-8-Emulator.git
cd CHIP-8-Emulator
```

### 2. Build the Project

Debug
```bash
cmake -B build
cmake --build build --config Debug
```

Release
```bash
cmake -B build
cmake --build build --config Release
```

### 3. Running a ROM
Run the compiled executable from your build directory:

```bash
# On Windows:
.\build\Release\CHIP-8.exe

# On Linux:
./build/CHIP-8
```