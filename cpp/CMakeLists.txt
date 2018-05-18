cmake_minimum_required(VERSION 3.10)
project(saoclib-cpp)

set(CMAKE_CXX_STANDARD 11)

# compile flags
execute_process(COMMAND aocl compile-config OUTPUT_VARIABLE compile_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${compile_flags}")
# link flags
execute_process(COMMAND aocl link-config OUTPUT_VARIABLE link_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${link_flags}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${link_flags}")

message(STATUS "using compile_flags: ${compile_flags}")
message(STATUS "using link flags: ${link_flags}")

include_directories($ENV{INTELFPGAOCLSDKROOT}/host/include)
include_directories($ENV{JAVA_HOME}/include
        $ENV{JAVA_HOME}/include/darwin
        $ENV{JAVA_HOME}/include/win32
        $ENV{JAVA_HOME}/include/linux)
include_directories(common/inc)
include_directories(src
        src/api
        src/cl
        src/kernelarg
        src/typetag
        src/utils)
include_directories(/usr/lib/jvm/default-java/)

set(libs rt pthread)
link_libraries(${libs})

file(GLOB utils "common/src/AOCLUtils/*.cpp" "src/utils/*")
file(GLOB typetag "src/typetag/*")
file(GLOB kernelarg "src/kernelarg/*")
file(GLOB cl "src/cl/*")
file(GLOB api "src/api/*")

set(srcs ${utils} ${typetag} ${kernelarg} ${cl})

# output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../bin)

# target executable
add_executable(matrix_mult.exe src/test/matrix_mult.cpp ${srcs})
add_executable(vector_add.exe src/test/vector_add.cpp ${srcs})

# target dynamic library
add_library(saoclib SHARED ${api} ${srcs})