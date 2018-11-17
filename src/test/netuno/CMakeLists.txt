cmake_minimum_required(VERSION 3.10.2)
project(INJECTOR_TEST)
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
include_directories(../..//include)

file(GLOB INJECTOR_TEST main.c ../../common/*.c ../../netuno/*.c)

add_executable(injector_test ${INJECTOR_TEST})

