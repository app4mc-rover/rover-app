#
# roverapp CMake file
# https://github.com/app4mc-rover/rover-app
#

#
# SETTINGS AND CMAKE CONFIG FINDING
#

cmake_minimum_required (VERSION 2.8.11)
project (roverapp)

# Use C++11 and Supress some warnings  -Wno-write-strings -Wno-deprecated-declarations -Wno-unused-local-typedefs

#set (CMAKE_CXX_FLAGS "-std=c++11 -Wno-write-strings -Wno-deprecated-declarations -Wno-unused-local-typedefs ${CMAKE_CXX_FLAGS}")
#set (CMAKE_C_FLAGS "-Wno-implicit-function-declaration ${CMAKE_C_FLAGS}")

set (CMAKE_CXX_FLAGS "-std=c++11 -Wno-write-strings -Wno-deprecated-declarations -Wno-unused-local-typedefs -O2 -pipe -g -feliminate-unused-debug-types  -fstack-protector-strong -pie -fpie -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -fvisibility-inlines-hidden ${CMAKE_CXX_FLAGS}")

# To find modules from other cmake-built projects that are searched with find_package,
# external cmake modules need to be provided. Those are typically named as Find<package>.cmake or <package>Config.cmake.
# Those must be added to the CMAKE_MODULE_PATH set below:
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/ExternalCMakeModules)

if(DEFINED ENV{SDKTARGETSYSROOT})
    message(STATUS "SDK Detected")
    set(SDK_EN 1)

    # Use OpenCV config from Yocto SDK (WA)
    set(OpenCV_DIR $ENV{SDKTARGETSYSROOT}/usr/share/OpenCV)
    # Raspicam module
    set(CMAKE_MODULE_PATH $ENV{SDKTARGETSYSROOT}/usr/lib/cmake ${CMAKE_MODULE_PATH})

else(DEFINED ENV{SDKTARGETSYSROOT})
    set(SDK_EN 0)
endif(DEFINED ENV{SDKTARGETSYSROOT})

#For find_package packages, export someextlib_DIR=/path/to/..
# Required packages
find_package (OpenCV REQUIRED)
find_package (raspicam REQUIRED)
find_package (Threads)


#Where to put binary files after "make"
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#Where to put library files after "make"
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)


if(${SIM})
    message(STATUS "Setting SIMULATION Defitnion to ${SIM}")
    message(STATUS "Compiling in simulator mode")
    add_definitions(-DSIMULATOR=1)
else()
    message(STATUS "Setting SIMULATION Defitnion to false")
    add_definitions(-DSIMULATOR=0)
endif()

add_definitions(-DCROSS_COMPILE_SDK=${SDK_EN})

#
# INCLUDE DIRECTORIES
#

#Include directories
include_directories (${CMAKE_SOURCE_DIR}/include)

#Include driver headers
include_directories (${CMAKE_SOURCE_DIR}/include/drivers/oled_drivers)

#Include directories for external libraries
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${raspicam_INCLUDE_DIRS} )
#include_directories( ${jsoncpp_INCLUDE_DIRS} )
#include_directories( ${bluetooth_INCLUDE_DIRS} )

#
# LIBRARIES
#

#Add roverapi
file (GLOB_RECURSE ROVERAPI_FILES
  ${CMAKE_SOURCE_DIR}/src/roverapi/*.c
  ${CMAKE_SOURCE_DIR}/src/roverapi/*.cpp
)
add_library (roverapi SHARED ${ROVERAPI_FILES})

set(CUSTOM_LIBS_DIR ${CMAKE_SOURCE_DIR}/src/libraries)

#Add our custom libraries
add_library (hono_interaction SHARED
  ${CUSTOM_LIBS_DIR}/hono_interaction/hono_interaction.cpp)
add_library (pthread_distribution SHARED
  ${CUSTOM_LIBS_DIR}/pthread_distribution_lib/pthread_distribution.cpp)
add_library (status_library SHARED
  ${CUSTOM_LIBS_DIR}/status_library/status_library.cpp)
add_library (pthread_monitoring SHARED
  ${CUSTOM_LIBS_DIR}/pthread_monitoring/collect_thread_name.cpp)
add_library (timing SHARED
  ${CUSTOM_LIBS_DIR}/timing/timing.cpp)
add_library (config_reader SHARED
  ${CUSTOM_LIBS_DIR}/config_reader/config_reader.cpp)


#
#  LINKING TO ROVER API
#
if(NOT DEFINED ${SIM})
    target_link_libraries (roverapi wiringPi)
    target_link_libraries (roverapi wiringPiDev)
endif()

target_link_libraries(roverapi hono_interaction)
target_link_libraries(roverapi pthread_distribution)
target_link_libraries(roverapi status_library)
target_link_libraries(roverapi pthread_monitoring)
target_link_libraries(roverapi timing)
target_link_libraries(roverapi config_reader)
target_link_libraries(roverapi paho-mqtt3a)
target_link_libraries(roverapi paho-mqtt3as)
target_link_libraries(roverapi paho-mqtt3c)
target_link_libraries(roverapi paho-mqtt3cs)
target_link_libraries(roverapi jsoncpp)

target_link_libraries (roverapi roverappdrivers)

#
# INSTALLING
#

#Install binary, shared library, includes, static library (archives)
install(
  TARGETS roverapi
    hono_interaction
    status_library
    config_reader
    pthread_monitoring
    timing pthread_distribution
  PUBLIC_HEADER
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

# Install public headers

file (GLOB_RECURSE ROVERAPI_INCLUDE_FILES
  ${CMAKE_SOURCE_DIR}/include/roverapi/*
)

file (GLOB_RECURSE ROVERLIBRARIES_INCLUDE_FILES
  ${CMAKE_SOURCE_DIR}/include/libraries/*
)

file (GLOB RECURSE ROVERDRIVERS_INCLUDE_FILES
  ${CMAKE_SOURCE_DIR}/include/drivers/*
)

INSTALL(FILES ${ROVERAPI_INCLUDE_FILES}
  DESTINATION include/roverapi
)

INSTALL(FILES ${ROVERDRIVERS_INCLUDE_FILES}
  DESTINATION include/drivers
)

INSTALL(FILES ${ROVERLIBRARIES_INCLUDE_FILES}
  DESTINATION include/libraries
)
