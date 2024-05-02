# Project settings
cmake_minimum_required(VERSION 3.15)
project(tcleaner-build
    VERSION 1.0
    DESCRIPTION "Build system for temp-cleaner app."
    LANGUAGES CXX
)

# Build settings
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build type: DEBUG")
    add_compile_definitions(DEBUG_TCLEANER)
else()
    message(STATUS "Build type: RELEASE")
endif()

# Compiling the main app
add_subdirectory(src)