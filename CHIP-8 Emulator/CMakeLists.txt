

set(mySources
    "src/emulator.cpp"
    "src/MMU.cpp"
 "src/cpu.cpp" "src/display.cpp" "src/CPU_opcodes.cpp")
add_executable(chip8 
    "main.cpp" ${mySources}
)

set_property(TARGET chip8 PROPERTY CXX_STANDARD 20)

target_link_libraries(chip8 PRIVATE SDL3-shared)

add_custom_command(TARGET chip8 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3-shared>
        $<TARGET_FILE_DIR:chip8>
    COMMENT "Copying SDL3.dll to output directory"
)

add_custom_command(TARGET chip8 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/roms"
        "$<TARGET_FILE_DIR:chip8>/roms"
    COMMENT "Copying test ROMs to the output directory..."
)