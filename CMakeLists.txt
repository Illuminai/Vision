cmake_minimum_required(VERSION 3.16)
project(Vision)

set(CMAKE_CXX_STANDARD 20)

include_directories("include")

FILE(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${VRC_RESOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Dependency Management
add_subdirectory(external)
add_subdirectory(assets)