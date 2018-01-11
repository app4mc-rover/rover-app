#
# roverapp CMake file
# https://github.com/app4mc-rover/rover-app
#

#
# SETTINGS AND CMAKE CONFIG FINDING
#

cmake_minimum_required (VERSION 2.8.11)
project (roverapp)

# Use C++11
set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

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

#Add tasks
#Add all files to a variable ROVERAPP_TASK_FILES
file (GLOB_RECURSE ROVERAPP_TASK_FILES ${CMAKE_SOURCE_DIR}/src/tasks/*.cpp)
add_library (roverapptasks SHARED ${ROVERAPP_TASK_FILES})


#Add drivers
file (GLOB_RECURSE ROVERAPP_DRIVER_FILES
  ${CMAKE_SOURCE_DIR}/src/drivers/oled_drivers/*.c
  ${CMAKE_SOURCE_DIR}/src/drivers/oled_drivers/*.cpp
)
add_library (roverappdrivers SHARED ${ROVERAPP_DRIVER_FILES})

#
#  EXECUTABLE
#

#Add main executable
add_executable(roverapp ${CMAKE_SOURCE_DIR}/src/roverapp.cpp)

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
target_link_libraries(roverapi paho-mqtt3a)
target_link_libraries(roverapi paho-mqtt3as)
target_link_libraries(roverapi paho-mqtt3c)
target_link_libraries(roverapi paho-mqtt3cs)
target_link_libraries(roverapi jsoncpp)

target_link_libraries (roverapi roverappdrivers)

#
#  LINKING TO ROVER TASKS
#

#Link external libraries
# Since OpenCV Module don't return full path for libraries 
target_link_libraries (roverapptasks ${OpenCV_LIB})

target_link_libraries (roverapptasks jsoncpp)
target_link_libraries (roverapptasks bluetooth)
target_link_libraries (roverapptasks pthread)

if(NOT DEFINED ${SIM})
    target_link_libraries (roverapptasks wiringPi)
    target_link_libraries (roverapptasks wiringPiDev)
    target_link_libraries (roverapptasks raspicam)
endif()

#Linker actions to link our libraries to executable
target_link_libraries(roverapptasks hono_interaction)
target_link_libraries(roverapptasks pthread_distribution)
target_link_libraries(roverapptasks status_library)
target_link_libraries(roverapptasks pthread_monitoring)
target_link_libraries(roverapptasks timing)

#Link api
target_link_libraries (roverapptasks roverapi)

#Link drivers
target_link_libraries (roverapptasks roverappdrivers)

#
# LINKING TO MAIN EXECUTABLE
#

#Link external libraries
target_link_libraries (roverapp pthread)

#Link api
target_link_libraries (roverapp roverapi)

#Linker actions to link our libraries to executable
target_link_libraries(roverapp hono_interaction)
target_link_libraries(roverapp pthread_distribution)
target_link_libraries(roverapp status_library)
target_link_libraries(roverapp pthread_monitoring)
target_link_libraries(roverapp timing)

#Link tasks
target_link_libraries(roverapp roverapptasks)

#Link drivers
target_link_libraries(roverapp roverappdrivers)

#
# EXAMPLES
#

add_executable(display_control ${CMAKE_SOURCE_DIR}/src/examples/display_control.cpp)
target_link_libraries (display_control roverapi)

add_executable(display_control ${CMAKE_SOURCE_DIR}/src/examples/driving_control.cpp)
target_link_libraries (driving_control roverapi)

add_executable(display_control ${CMAKE_SOURCE_DIR}/src/examples/mqtt_telemetry.cpp)
target_link_libraries (mqtt_telemetry roverapi)

add_executable(display_control ${CMAKE_SOURCE_DIR}/src/examples/sensor_reading.cpp)
target_link_libraries (sensor_reading roverapi)

add_executable(display_control ${CMAKE_SOURCE_DIR}/src/examples/utility_info.cpp)
target_link_libraries (utility_info roverapi)


#
# INSTALLING
#

#Install binary, shared library, static library (archives)
install(
  TARGETS roverapp roverapi
    roverapptasks
    roverappdrivers
    hono_interaction
    status_library
    pthread_monitoring
    timing pthread_distribution
  PUBLIC_HEADER
  RUNTIME DESTINATION /usr/bin
  LIBRARY DESTINATION /usr/lib
  ARCHIVE DESTINATION /usr/lib)

#Copy/install files to a certain static directory
install(FILES ${CMAKE_SOURCE_DIR}/scripts/read_core_usage.py
  DESTINATION /opt/rover-app/scripts/)
