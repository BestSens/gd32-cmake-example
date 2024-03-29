set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(MINGW OR CYGWIN OR WIN32)
    set(UTIL_SEARCH_CMD where)
elseif(UNIX OR APPLE)
    set(UTIL_SEARCH_CMD which)
endif()

set(TOOLCHAIN_PREFIX arm-none-eabi-)

execute_process(
  COMMAND ${UTIL_SEARCH_CMD} ${TOOLCHAIN_PREFIX}gcc
  OUTPUT_VARIABLE BINUTILS_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)

set(CMAKE_AR ${TOOLCHAIN_PREFIX}gcc-ar)
set(CMAKE_C_COMPILER_AR ${CMAKE_AR})
set(CMAKE_CXX_COMPILER_AR ${CMAKE_AR})
set(CMAKE_ASM_COMPILER_AR ${CMAKE_AR})

set(CMAKE_RANLIB ${TOOLCHAIN_PREFIX}gcc-ranlib)
set(CMAKE_C_COMPILER_RANLIB ${CMAKE_RANLIB})
set(CMAKE_CXX_COMPILER_RANLIB ${CMAKE_RANLIB})
set(CMAKE_ASM_COMPILER_RANLIB ${CMAKE_RANLIB})

set(CMAKE_NM ${TOOLCHAIN_PREFIX}gcc-nm)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size)

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)