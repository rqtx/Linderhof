cmake_minimum_required(VERSION 3.10.2)
project(LINDERHOF)
enable_language(C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_BINARY_DIR ${SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS_RELEASE "-O0")


#add_compile_definitions("ORYXNET")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g -Wl,--no-as-needed -lpthread -lcap -lm -D _GNU_SOURCE")
include_directories(src/include)

file(GLOB LINDERHOF src/linderhof/main.c src/linderhof/*/*.c src/interface/interface.c src/interface/oryx.c src/linderhof/hom/*/src/*.c src/common/*.c src/netuno/*.c)
#file(GLOB SNOW .c src/common/*.c src/interface/configr.c src/interface/interface.c src/netuno/logger.c src/monitor/*.c src/interface/snowman.c )

add_executable(lhf ${LINDERHOF})
#add_executable(snowman ${SNOW})

