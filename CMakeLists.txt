#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.17)

# specify cross compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# version major update  0..255
set(VERSION_FIRMWARE_MAJOR 1)

# version mainor update 0..255
set(VERSION_FIRMWARE_MINOR 6)

# project settings
project(CS_EPS_PMM_N_SH_v C CXX ASM)

set( PROJECT_NAME ${PROJECT_NAME}_${VERSION_FIRMWARE_MAJOR}_${VERSION_FIRMWARE_MINOR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()


include_directories(Drivers/CMSIS/Include Core/Inc Core/Inc/PMM Core/Inc/PAM Core/Inc/PDM Core/Inc/PBM Core/Inc/CAND Core/Inc/FLASH Drivers/CMSIS/Device/ST/STM32L4xx/Include Drivers/STM32L4xx_HAL_Driver/Inc)

add_definitions(-DHSE_VALUE=16000000 -DMSI_VALUE=4000000 -DLSI_VALUE=32000 -DHSE_STARTUP_TIMEOUT=100 -DHSI_VALUE=16000000 -DDEBUG -DDATA_CACHE_ENABLE=1 -DVDD_VALUE=3300 -DINSTRUCTION_CACHE_ENABLE=1 -DEXTERNALSAI2_CLOCK_VALUE=2097000 -DUSE_FULL_LL_DRIVER -DEXTERNALSAI1_CLOCK_VALUE=2097000 -DPREFETCH_ENABLE=0 -DSTM32L496xx -DVERSION_FW_MAJOR=${VERSION_FIRMWARE_MAJOR} -DVERSION_FW_MINOR=${VERSION_FIRMWARE_MINOR} -DDEBUGprintf)

file(GLOB_RECURSE SOURCES "Core/*.*" "Drivers/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32L496QGIX_FLASH.ld)

add_library(Flash_lib STATIC IMPORTED GLOBAL)
set_target_properties( Flash_lib PROPERTIES IMPORTED_LOCATION  ${CMAKE_SOURCE_DIR}/Core/Src/LIB/libflash_lib.a )

add_library(canv_lib STATIC IMPORTED GLOABAL)
set_target_properties( canv_lib PROPERTIES IMPORTED_LOCATION  ${CMAKE_SOURCE_DIR}/Core/Src/LIB/libcanv_lib.a )


add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb  -specs=nano.specs -specs=nosys.specs -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}.elf canv_lib)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}.elf Flash_lib)


set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
