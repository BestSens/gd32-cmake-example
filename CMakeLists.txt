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

add_definitions(
	-DGD32F450
	-DGD32F450Z_EVAL
)

add_compile_options(
	-mcpu=cortex-m4
	-mthumb
	$<$<CONFIG:Debug>:-Og>
	$<$<CONFIG:Release>:-O3>
	$<$<CONFIG:MinSizeRel>:-Os>
	$<$<CONFIG:RelWithDebInfo>:-O3>
)

add_link_options(
	-mcpu=cortex-m4
)

add_subdirectory(Examples)
