# Project settings
cmake_minimum_required(VERSION 3.15)
project(tcleaner-build-src
    VERSION 1.0
    DESCRIPTION "Build system for temp-cleaner source code."
    LANGUAGES CXX
)

# Set compiler options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# File variables
file(GLOB_RECURSE INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/*.hpp)
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp)

# Creating the main executable
set(APP "temp-cleaner")
set(LIB "temp-cleaner-static")
add_executable(${APP} ${SRC_FILES} ${INC_FILES})

# Adding specific compiler flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(COMPILE_FLAGS "/Wall /Yd /Oy /Gw")
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(COMPILE_FLAGS "-Wall -Wextra -pedantic -Wno-reorder -fno-omit-frame-pointer -fdata-sections -g")
    else()
        set(COMPILE_FLAGS "-Wall -Wextra -pedantic -Wno-reorder -fdata-sections")
    endif()
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

# Format the code using clang-format
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(FORMAT "Format the code using clang-format" ON)
    if(FORMAT)
        find_program(CLANG_FORMAT_EXECUTABLE clang-format)
        if(CLANG_FORMAT_EXECUTABLE)
            message( STATUS "clang-format found: ${CLANG_FORMAT_EXECUTABLE}")
            file(GLOB_RECURSE SOURCE_FILES
                ${CMAKE_SOURCE_DIR}/src/app.cpp
                ${CMAKE_SOURCE_DIR}/include/tcleaner/*.hpp
                ${CMAKE_SOURCE_DIR}/src/*.cpp
            )
            add_custom_target(format
                COMMAND ${CLANG_FORMAT_EXECUTABLE}
                -i
                ${SOURCE_FILES}
            )
            add_dependencies(${APP} format)
        else()
            message(STATUS "clang-format not found. Skipping code formatting.")
        endif()
    endif()
endif()

# Adding cppcheck properties
find_program(CPPCHECK_FOUND cppcheck)
if (CPPCHECK_FOUND AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(cppcheck_options "--enable=warning" "--inconclusive" "--force" "--inline-suppr")
    set_target_properties(${APP} PROPERTIES CXX_CPPCHECK "${CPPCHECK_FOUND}")
endif()

# Install
include(GNUInstallDirs)
install(TARGETS ${APP} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})