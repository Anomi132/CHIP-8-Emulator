
add_executable(chip8 
    "CHIP-8 Emulator.cpp" 
    "CHIP-8 Emulator.h"
)

set_property(TARGET chip8 PROPERTY CXX_STANDARD 20)

target_link_libraries(chip8 PRIVATE SDL3-static)
