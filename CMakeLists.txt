cmake_minimum_required(VERSION 3.8)

project("CHIP-8 Emulator")

add_subdirectory(external/SDL2)

add_subdirectory("CHIP-8 Emulator")

target_include_directories(chip8 PRIVATE 
    "${CMAKE_SOURCE_DIR}/external/SDL2/include"
)

target_include_directories(chip8 PRIVATE 
    "${CMAKE_SOURCE_DIR}/Chip-8 Emulator/include"
)
