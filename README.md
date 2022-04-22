# GD32 CMake example

**THIS PROJECT IS CURRENTLY IN A NON-WORKING STATE AS I DON'T YET HAVE HARDWARE TO TEST THE BUILDS**

Goal of this repo is to provide an example CMake project to build the GigaDevice Demo suites provided for the GD32F4xx derivates with the standard GNU Arm Embedded Toolchain.

Currently it's just a copy of the Demo Suites package with added CMake files, a toolchain configuration and a converted startup asm to be compatible with the GNU compiler.

## Dependencies
To build this package you need a working GNU Arm Embedded Toolchain (tested with 10.3-2021.10), CMake and a build system installed. I'm using Ninja in my examples.

I'm using this on windows but it may work on Linux or macOS as well.

## Usage
The demo applications can be built with the following commands:

```PowerShell
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE="cmake/arm-none-eabi-gcc.cmake" -DCMAKE_GENERATOR="Ninja Multi-Config"
cmake --build .\build\ --config Release
```