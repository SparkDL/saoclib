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
include_directories(host
        host/api
        host/cl
        host/kernelarg
        host/typetag
        host/utils)
include_directories(/usr/lib/jvm/default-java/)

set(libs rt pthread)
link_libraries(${libs})

file(GLOB utils "common/src/AOCLUtils/*.cpp" "host/utils/*")
file(GLOB typetag "host/typetag/*")
file(GLOB kernelarg "host/kernelarg/*")
file(GLOB cl "host/cl/*")
file(GLOB api "host/api/*")
file(GLOB test "host/test/*")

set(srcs ${utils} ${typetag} ${kernelarg} ${cl})

# output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)

# target executable
add_executable(matrix_mult.exe host/test/matrix_mult.cpp ${srcs})
add_executable(vector_add.exe host/test/vector_add.cpp ${srcs})
add_executable(two_vec_ops.exe host/test/two_vec_ops.cpp ${srcs})
add_executable(single_vec_ops.exe host/test/single_vec_ops.cpp ${srcs})
add_executable(sger.exe host/test/sger.cpp ${srcs})

# target dynamic library
add_library(aoclmkl SHARED ${api} ${srcs})