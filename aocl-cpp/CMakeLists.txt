cmake_minimum_required(VERSION 3.10)
project(vector_add)

set(CMAKE_CXX_STANDARD 11)

# compile flags
execute_process(COMMAND aocl compile-config OUTPUT_VARIABLE compile_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${compile_flags}" )
# link flags
execute_process(COMMAND aocl link-config OUTPUT_VARIABLE link_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${link_flags}" )

message(STATUS "using compile_flags: ${compile_flags}")
message(STATUS "using link flags: ${link_flags}")

include_directories($ENV{INTELFPGAOCLSDKROOT}/host/include )
include_directories(common/inc)

set(libs rt pthread)
link_libraries(${libs})

file(GLOB utils "common/src/AOCLUtils/*.cpp")
set(srcs host/src/core.cpp ${utils} host/src/Kernel.cpp host/src/Kernel.h)

# output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../bin)

# target executable
# add_executable(host ${srcs})

add_executable(core ${srcs})