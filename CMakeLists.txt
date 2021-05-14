# Project Globals
project(TM4C123GXL)
set(TIVAWARE_PATH ${PROJECT_SOURCE_DIR}/../tivaware)

set (PROJECT_ELF ${CMAKE_PROJECT_NAME}.elf)
set (PROJECT_BIN ${CMAKE_PROJECT_NAME}.bin)

cmake_minimum_required(VERSION 3.8.2)

# Set Compilation information
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Include
include(cmake/tm4c123.cmake)

# Export Compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes
set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    ${TIVAWARE_PATH}/inc
    ${TIVAWARE_PATH}/utils
    ${TIVAWARE_PATH}/driverlib
)

include_directories(${PROJECT_INCLUDES})

# Sources
file (GLOB USER_SOURCES src/*.c)
file (GLOB TIVA_SOURCES ${TIVAWARE_PATH}/driverlib/*.c)

set (PROJECT_SOURCES
    ${USER_SOURCES}
    )

# Executable
add_executable(${PROJECT_ELF} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_ELF}
#     ${TIVAWARE_PATH}/usblib/gcc/libusb.a
#     ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a
)

# Flash
set(FLASH_EXECUTABLE "lm4flash")
ADD_CUSTOM_TARGET("flash" DEPENDS ${PROJECT_ELF}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_ELF} ${PROJECT_BIN}
    COMMAND ${FLASH_EXECUTABLE} ${PROJECT_BIN}
)

# Debug
set(OPENOCD_BOARD "/usr/share/openocd/scripts/board/ek-tm4c123gxl.cfg")
ADD_CUSTOM_TARGET(
    "debug"
    COMMAND openocd -f ${OPENOCD_BOARD} >/dev/null 2>&1 & sleep 2
    COMMAND arm-none-eabi-gdb -quiet -tui -command=${PROJECT_SOURCE_DIR}/gdb/GDBCommands -se ${PROJECT_ELF}
    COMMAND killall -15 openocd
)
