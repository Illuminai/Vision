cmake_minimum_required(VERSION 3.6)
project(CMakeRC VERSION 2.0.0)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake")

include(CMakeRC.cmake)
