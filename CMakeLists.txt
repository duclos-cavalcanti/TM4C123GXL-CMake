project(TM4C123GXL)
cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tm4c/Tiva Cmake
set(TIVAWARE_PATH ${PROJECT_SOURCE_DIR}/../tivaware)
include(cmake/tm4c123g.cmake)

set (PROJECT_EXEC ${CMAKE_PROJECT_NAME}.axf)
set (PROJECT_BIN ${CMAKE_PROJECT_NAME}.bin)

# Includes
set(PROJECT_INCLUDES
    ${PROJECT_SOURCE_DIR}/include
    ${TIVAWARE_PATH}/inc
)
include_directories(${PROJECT_INCLUDES})

# Sources
file (GLOB USER_SOURCES src/*.c)
set (PROJECT_SOURCES
    ${USER_SOURCES}
    )

# Executable
add_executable(${PROJECT_EXEC} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_EXEC}
    ${TIVAWARE_PATH}/usblib/gcc/libusb.a
    ${TIVAWARE_PATH}/driverlib/gcc/libdriver.a
)

# Flash
set(FLASH_EXECUTABLE "lm4flash")
ADD_CUSTOM_TARGET("flash" DEPENDS ${PROJECT_EXEC}
  COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_EXEC} ${PROJECT_BIN}
  COMMAND ${FLASH_EXECUTABLE} ${PROJECT_BIN}
)

# Debug
set(OPENOCD_BOARD "/usr/share/openocd/scripts/board/ek-tm4c123gxl.cfg")
ADD_CUSTOM_TARGET(
    "debug"
    COMMAND openocd -f ${OPENOCD_BOARD} >/dev/null 2>&1 & sleep 2
    COMMAND arm-none-eabi-gdb -quiet -tui -command=${PROJECT_SOURCE_DIR}/gdb/GDBCommands -se ${PROJECT_EXEC}
    COMMAND killall -15 openocd
)
