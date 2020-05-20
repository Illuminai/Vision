cmake_minimum_required(VERSION 3.16)
project(Vision)

set(CMAKE_CXX_STANDARD 20)

FILE(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")
include_directories("include")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Dependency Management
add_subdirectory(external)