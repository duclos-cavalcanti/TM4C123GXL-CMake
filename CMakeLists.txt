project(embedded-shape-world)

cmake_minimum_required(VERSION 3.8.2)

# Tm4c/Tiva Cmake
include(cmake/tm4c123g.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TIVAWARE_PATH ${PROJECT_SOURCE_DIR}/../tivaware)
set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/tm4c
    ${PROJECT_SOURCE_DIR}/../Labware/inc
    ${TIVAWARE_PATH}
)
include_directories(${PROJECT_INCLUDES})

file (GLOB USER_SOURCES src/*.c)
set (PROJECT_SOURCES
    ${USER_SOURCES}
    )

add_executable(${CMAKE_PROJECT_NAME}.axf ${PROJECT_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME}.axf 
    ${TIVAWARE_PATH}/usblib/gcc/libusb.a
    ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a
)

# Flash
set(FLASH_EXECUTABLE "lm4flash")
ADD_CUSTOM_TARGET("flash" DEPENDS ${CMAKE_PROJECT_NAME}.axf 
  COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.axf ${CMAKE_PROJECT_NAME}.bin 
  COMMAND ${FLASH_EXECUTABLE} ${CMAKE_PROJECT_NAME}.bin
)

# Debug
set(OPENOCD_BOARD "/usr/share/openocd/scripts/board/ek-tm4c123gxl.cfg")
ADD_CUSTOM_TARGET(
    "debug"
    COMMAND openocd -f ${OPENOCD_BOARD} >/dev/null 2>&1 & sleep 2
    COMMAND arm-none-eabi-gdb -quiet -tui -command=${PROJECT_SOURCE_DIR}/gdb/GDBCommands -se ${CMAKE_PROJECT_NAME}.axf 
    COMMAND killall -15 openocd
)
