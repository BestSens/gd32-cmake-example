# GD32 CMake example
Goal of this repo is to provide an example CMake project to build the GigaDevice Demo suites provided for the GD32F4xx derivates with the standard GNU Arm Embedded Toolchain.

Currently it's just a copy of the Demo Suites package with added CMake files, a toolchain configuration, a converted startup asm to be compatible with the GNU compiler and a linker script.

I've also commented out the documentation building for lwip in the ENET example to don't require doxygen to configure.

I've removed '&'s from various Example paths as they are note compatible with CMake and did some fixes that break compilation on Unix based systems.

## Dependencies
To build this package you need a working GNU Arm Embedded Toolchain (tested with 11.3-rel1 and 12.2.MPACBTI-Bet1), CMake and a build system installed. I'm using Ninja in my examples.

I've tested this on Windows and Linux but with a bit of "you're lucky" it may work on macOS as well.

## Usage
All demo applications can be built with the following commands:

```PowerShell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="cmake/arm-none-eabi-gcc.cmake" -DCMAKE_GENERATOR="Ninja Multi-Config"
cmake --build .\build\ --config Release
```

To build only specific examples, use the following to build:

```PowerShell
cmake --build .\build\ --config Release --target Running_led.bin
```

## Known issues
* As each example may add a different configuration file to the libraries every target needs to be built for each example
* Linking and include directories are odd because of the above
* Amount of targets get enormous because of the above, thus generating take long
* There may be errors because of too long paths
* Definitions should be local to targets not global
* Board should be selectable with variables instead of building everything for the gd32f450z-eval excluding the ones specifically for the gd32f450i-eval
* USB-Device examples are broken since firmware library update to 3.2.0 and I don't think it's my fault
* Retargeting only works by providing custom _write() override due to newlib used in arm-none-eabi-gcc
* Linking on gcc 11.2 fails when retargeting
