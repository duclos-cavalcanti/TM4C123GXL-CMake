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
set(LNK "")                                     # Linking Flags

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mthumb ${CPU} ${FPU} -MD")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb ${CPU} ${FPU} ${OPT} -MD -std=gnu99 -Os -pedantic ${DBG}")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "-L ${TIVAWARE_PATH}/driverlib -T ${PROJECT_SOURCE_DIR}/link/tm4c123g.ld -specs=${PROJECT_SOURCE_DIR}/specs/tiva.specs")

# Processor specific definitions
add_definitions(-DPART_${PART})
add_definitions(-DTARGET_IS_${TARGET})
add_definitions(-Dgcc)

