cmake_minimum_required(VERSION 3.10.3)
project(LINDERHOF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set( SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

set(CMAKE_BINARY_DIR ${SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall -g -lpthread -D _DEFAULT_SOURCE")
include_directories(src/include)

set(LINDERHOF_SOURCES 
  src/main.c

  src/common/error.c
  src/common/memutils.c
  src/common/netio.c

  src/commander/linderhof/linderhof.c
  src/commander/linderhof/hom.c
  src/commander/linderhof/planner.c
)


add_executable(lhf ${LINDERHOF_SOURCES})
