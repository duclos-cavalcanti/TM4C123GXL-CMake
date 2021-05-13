# Project Globals
project(TM4C123GXL)
set(TIVAWARE_PATH ${PROJECT_SOURCE_DIR}/../tivaware)

set (PROJECT_ELF ${CMAKE_PROJECT_NAME}.elf)
set (PROJECT_BIN ${CMAKE_PROJECT_NAME}.bin)

cmake_minimum_required(VERSION 3.8.2)

# Set Compilation information
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set (PART TM4C123GH6PM)
set (TARGET TM4C123_RB1) # or RA1

# GCC toolchain prefix
set(TOOLCHAIN_PREFIX "arm-none-eabi")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-as)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)

enable_language(ASM)
set(CPU "-mcpu=cortex-m4")                      # CPU FLags
set(FPU "-mfpu=fpv4-sp-d16 -mfloat-abi=softfp") # FPU Flags
set(OPT "-ffunction-sections -fdata-sections")  # Optimization Flags
set(DBG "-g -Wall")                             # Debug/Warning Flags
set(LNK "       ")                              # Linking Flags

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mthumb ${CPU} ${FPU} -MD")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb ${CPU} ${FPU} ${OPT} -std=gnu99 -MD -Os -pedantic ${DBG}")

#set(CMAKE_EXE_LINKER_FLAGS "-T${PROJECT_SOURCE_DIR}/link/tm4c123g.ld -specs=${PROJECT_SOURCE_DIR}/specs/tiva.specs")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "-T ${PROJECT_SOURCE_DIR}/link/tm4c123g.ld --entry ResetIntHandler")

# Processor specific definitions
add_definitions(-DPART_${PART})
add_definitions(-DTARGET_IS_${TARGET})
add_definitions(-Dgcc)

# Export Compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes
set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    ${TIVAWARE_PATH}/inc
    ${TIVAWARE_PATH}/driverlib
)

include_directories(${PROJECT_INCLUDES})

# Sources
file (GLOB USER_SOURCES src/*.c)
file (GLOB TIVA_SOURCES ${TIVAWARE_PATH}/driverlib/*.c)

set (PROJECT_SOURCES
    ${USER_SOURCES}
    ${TIVA_SOURCES}
    )

# Executable
add_executable(${PROJECT_ELF} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_ELF}
    ${TIVAWARE_PATH}/usblib/gcc/libusb.a
    ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a
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
