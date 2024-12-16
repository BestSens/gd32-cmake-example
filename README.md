# GD32 CMake example
Goal of this repo is to provide an example CMake project to build the GigaDevice Demo suites provided for the GD32F4xx derivates with the standard GNU Arm Embedded Toolchain.

Currently it's just a copy of the Demo Suites package with added CMake files, a toolchain configuration, c++ startup code to be compatible with the GNU compiler and a linker script. I also did some bugfixes on the ADC examples, they were not compatible with the firmware starting with v3.0.2.

I've also commented out the documentation building for lwip in the ENET example to don't require doxygen to configure.

I've removed '&'s from various Example paths as they are note compatible with CMake and did some fixes that break compilation on Unix based systems.

## Dependencies
To build this package you need a working GNU Arm Embedded Toolchain (tested with 14.2.Rel1), CMake and a build system installed. I'm using Ninja in my examples.

I've tested this on Windows and Linux but with a bit of luck it may work on macOS as well.

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

## OpenOCD
This project also contains OpenOCD configuration files to access the GD32F450 via a standard CMSIS-DAP interface (the GD-Link one on the GD32450i-EVAL), J-Link or JTAG over an Altera USB-Blaster. These configuration files can easily be changed for the used interface.

The `.vscode/tasks.json` also contains examples of how to use OpenOCD to flash or erase the target. Tested with OpenOCD 0.11.0.

**The GD32F450 configuration file is using the stm32f4x driver for accessing the on-board flash. As STM32 devices have a maximum of 2048 KB of flash, I needed to override this size. This is somewhat working but page sizes are wrong starting at offset 1020 KB. So OpenOCD may delete wrong pages eg. may delete not enough so data written is corrupt. Should be fine when only using the first 1024 KB of flash.**

## Known issues
* As each example may add a different configuration file to the libraries every target needs to be built for each example
* Linking and include directories are odd because of the above
* Amount of targets get enormous because of the above, thus generating take long
* There may be errors because of too long paths
* Definitions should be local to targets not global
* Board should be selectable with variables instead of building everything for the gd32f450z-eval excluding the ones specifically for the gd32f450i-eval
* USB-Device examples are broken since firmware library update to v3.0.2 and I don't think it's my fault
* Retargeting only works by providing custom _write() override due to newlib used in arm-none-eabi-gcc
* Linking on gcc 11.2 fails when retargeting
* GigaDevice broke many DAC examples with firmware library v3.2.0
* Linker will warn on unimplemented functions
