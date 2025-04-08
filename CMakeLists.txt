cmake_minimum_required(VERSION 3.17.0)
project(gd32-cmake-example)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_LTO "Enable LTO" ON)

if(USE_LTO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT supported OUTPUT error)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if (isMultiConfig)
	set(CMAKE_CROSS_CONFIGS "Release;Debug")
	set(CMAKE_DEFAULT_BUILD_TYPE "Release")
	set(CMAKE_DEFAULT_CONFIGS "Release;Debug")
endif ()

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12.0)
add_link_options(
	-Wl,--no-warn-rwx-segments
)
endif()

add_definitions(
	-DGD32F450
	-DGD32F450I_EVAL
)

add_compile_options(
	-mcpu=cortex-m4
	-mfloat-abi=hard
	-specs=nosys.specs

	-ffunction-sections
	-fdata-sections
	-fstack-usage
	-ffast-math

	"$<$<CONFIG:Debug>:-Og;-DDEBUG;-g;-funwind-tables>"
	"$<$<CONFIG:Release>:-O2;-DNDEBUG>"
	"$<$<CONFIG:MinSizeRel>:-Os;-DNDEBUG>"
	"$<$<CONFIG:RelWithDebInfo>:-O2;-g;-DNDEBUG>"
)

add_link_options(
	-mcpu=cortex-m4
	-mfloat-abi=hard
	-specs=nosys.specs
	-static
	-u _printf_float
	-Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	-Wl,--gc-sections
	-T${PROJECT_SOURCE_DIR}/gd32f450.ld
)

set(CMAKE_EXECUTABLE_SUFFIX .elf)

add_subdirectory(Examples)
