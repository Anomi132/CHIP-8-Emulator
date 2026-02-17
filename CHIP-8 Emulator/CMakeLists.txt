# CMakeList.txt : CMake project for CHIP-8 Emulator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CMakeTarget "CHIP-8 Emulator.cpp" "CHIP-8 Emulator.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
