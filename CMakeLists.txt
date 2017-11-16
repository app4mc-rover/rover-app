#
# roverapp CMake file
# https://github.com/app4mc-rover/rover-app
#

#
# SETTINGS AND CMAKE CONFIG FINDING
#

cmake_minimum_required (VERSION 2.8.11)
project (roverapp)

#For find_package packages, export someextlib_DIR=/path/to/..

# Required packages
find_package (OpenCV REQUIRED)
find_package (raspicam REQUIRED)
find_package (Threads)

#Where to put binary files after "make"
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#Where to put library files after "make"
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/libs)

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

#Add api
add_library(roverapi SHARED ${CMAKE_SOURCE_DIR}/src/api/basic_psys_rover.c)

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
#  LINKING TO ROVER TASKS
#

#Link external libraries
# Search with sudo find / -name *libx*
# If there is an x.so in /usr/lib/ or /usr/local/lib/, you can link x library
target_link_libraries (roverapptasks opencv_core)
target_link_libraries (roverapptasks opencv_imgproc)
target_link_libraries (roverapptasks opencv_highgui)
target_link_libraries (roverapptasks jsoncpp)
target_link_libraries (roverapptasks bluetooth)
target_link_libraries (roverapptasks pthread)
target_link_libraries (roverapptasks wiringPi)
target_link_libraries (roverapptasks wiringPiDev)
target_link_libraries (roverapptasks raspicam)


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
target_link_libraries (roverapp roverappdrivers)

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
