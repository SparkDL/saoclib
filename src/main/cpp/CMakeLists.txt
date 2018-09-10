cmake_minimum_required(VERSION 3.10)
project(aclmkl-cpp)

set(CMAKE_CXX_STANDARD 11)

set(release_dir ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)

# gtest build
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(gtest)


# compile flags
execute_process(COMMAND aocl compile-config OUTPUT_VARIABLE compile_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${compile_flags}")
# link flags
execute_process(COMMAND aocl link-config OUTPUT_VARIABLE link_flags OUTPUT_STRIP_TRAILING_WHITESPACE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${link_flags}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${link_flags}")

message(STATUS "Using compile_flags: ${compile_flags}")
message(STATUS "Using link flags: ${link_flags}")
message(STATUS "Using JAVA_HOME: $ENV{JAVA_HOME}")

# inlucde headers
include_directories($ENV{INTELFPGAOCLSDKROOT}/host/include)
include_directories($ENV{JAVA_HOME}/include
        $ENV{JAVA_HOME}/include/darwin
        $ENV{JAVA_HOME}/include/win32
        $ENV{JAVA_HOME}/include/linux)
include_directories(common/inc)
include_directories(device/kernels
        host
        host/aclmkl
        host/java
        host/acl
        host/kernelarg
        host/typetag
        host/utils)

set(libs rt pthread)
link_libraries(${libs})

file(GLOB utils "common/src/AOCLUtils/*" "common/inc/AOCLUtils/*" "host/utils/*" "device/kernels/config.h")
file(GLOB cl "host/acl/*")
file(GLOB blas "host/aclmkl/*")
file(GLOB test "test/*")

set(srcs "host/acl/acl.h" ${utils} ${typetag} ${kernelarg} ${cl} ${blas})

# output directory
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../../../target/classes)

# target executable
add_executable(test.exe ${srcs} ${test})

target_link_libraries(test.exe gtest_main)

# tests
enable_testing()
add_test(NAME test
        COMMAND test.exe)

add_library(aclmkl SHARED ${api} ${srcs} ${blas})

# Copy the share library to the "classes" directory
# to ensure the released jar contains the libaclmkl.so file.
#ADD_CUSTOM_TARGET(copy_library ALL
#        COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/libaclmkl.so ${release_dir}
#        DEPENDS aclmkl)