cmake_minimum_required(VERSION 3.10)

project(p VERSION 1.0
        DESCRIPTION "Chess with database"
        LANGUAGES CXX)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE source/database.cpp source/mainFrame.cpp source/app.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

# Detect the target platform
if(WIN32)
    set(IS_WINDOWS TRUE)
elseif(UNIX AND NOT APPLE)
    set(IS_LINUX TRUE)
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

# Find SQLite package
find_package(SQLite3 REQUIRED)

# Link SQLite library
target_link_libraries(${PROJECT_NAME} SQLite::SQLite3)

# Find wxWidgets package
find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

# Link wxWidgets library
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

# System difference
if(IS_WINDOWS)

    # Add compiler and linker flags through Visual Studio project
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)  # Treat warnings as errors (/WX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)  # Use Unicode character set

    # Add optimization flags based on build type
    set(COMMON_FLAGS "/arch:AVX2 /Oi")  # Use AVX2 instruction set and enable intrinsic functions
    set(DEBUG_FLAGS "/Od /Zi")  # Disable optimizations and include debugging information
    set(RELEASE_FLAGS "/O2")  # Optimize for maximum speed

    # Set different optimization flags for Debug and Release builds
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS_DEBUG ${COMMON_FLAGS} ${DEBUG_FLAGS})
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS_RELEASE ${COMMON_FLAGS} ${RELEASE_FLAGS})
    endif()

elseif(IS_LINUX)

    # Linux-specific configurations
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_link_options(${PROJECT_NAME} PRIVATE -pthread)

    # Add optimization flags based on build type
    set(COMMON_FLAGS "-march=native")

    # Set different optimization flags for Debug and Release builds
    set(DEBUG_FLAGS "-Og -g")
    set(RELEASE_FLAGS "-O3")

    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_FLAGS} ${DEBUG_FLAGS})
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_FLAGS} ${RELEASE_FLAGS})
    endif()
    
endif()
