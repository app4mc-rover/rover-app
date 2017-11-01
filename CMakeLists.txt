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
#find_package (raspicam REQUIRED)
find_package (Threads)

#Where to put binary files after "make"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#Where to put library files after "make"
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)

#
# INCLUDE DIRECTORIES
#

#Include directories
include_directories (${PROJECT_BINARY_DIR}/../include)

#Include driver headers
include_directories (${PROJECT_BINARY_DIR}/../include/drivers/oled_drivers)

#Include directories for external libraries
include_directories( ${OpenCV_INCLUDE_DIRS} )
#include_directories( ${jsoncpp_INCLUDE_DIRS} )
#include_directories( ${bluetooth_INCLUDE_DIRS} )

#
# LIBRARIES
#

#Add api
add_library(roverapi SHARED ${PROJECT_BINARY_DIR}/../src/api/basic_psys_rover.c)

#Add our custom libraries
add_library (hono_interaction SHARED ${PROJECT_BINARY_DIR}/../src/libraries/hono_interaction/hono_interaction.cpp)
add_library (pthread_distribution SHARED ${PROJECT_BINARY_DIR}/../src/libraries/pthread_distribution_lib/pthread_distribution.cpp)
add_library (status_library SHARED ${PROJECT_BINARY_DIR}/../src/libraries/status_library/status_library.cpp)
add_library (pthread_monitoring SHARED ${PROJECT_BINARY_DIR}/../src/libraries/pthread_monitoring/collect_thread_name.cpp)
add_library (timing SHARED ${PROJECT_BINARY_DIR}/../src/libraries/timing/timing.cpp)

#Add tasks
#Add all files to a variable ROVERAPP_TASK_FILES
file (GLOB_RECURSE ROVERAPP_TASK_FILES ${PROJECT_BINARY_DIR}/../src/tasks/*cpp)
add_library (roverapptasks SHARED ${ROVERAPP_TASK_FILES})


#Add drivers
file (GLOB_RECURSE ROVERAPP_DRIVER_FILES ${PROJECT_BINARY_DIR}/../src/drivers/oled_drivers/*)
add_library (roverappdrivers SHARED ${ROVERAPP_DRIVER_FILES})

#
#  EXECUTABLE
#

#Add main executable
add_executable(roverapp ${PROJECT_BINARY_DIR}/../src/roverapp.cpp)

#
#  LINKING TO ROVER TASKS
#

#Link external libraries
# Search with sudo find / -name *libx*
# If there is an x.so, you can link x library
target_link_libraries (roverapptasks opencv_core)
target_link_libraries (roverapptasks opencv_imgproc)
target_link_libraries (roverapptasks opencv_highgui)
target_link_libraries (roverapptasks jsoncpp)
target_link_libraries (roverapptasks bluetooth)
target_link_libraries (roverapptasks pthread)
target_link_libraries (roverapptasks wiringPi)
target_link_libraries (roverapptasks wiringPiDev)
#target_link_libraries (roverapptasks raspicam)


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

#Post-build actions, to copy a file to a directory in 'make'
#add_custom_command (
#        TARGET roverapp POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${PROJECT_BINARY_DIR}/../scripts/read_core_usage.py
#        /opt/
#)

#
# INSTALLING
#

#Install binary, shared library, static library (archives)
install (TARGETS roverapp roverapi roverapptasks roverappdrivers hono_interaction status_library pthread_monitoring timing pthread_distribution
  RUNTIME DESTINATION  ${PROJECT_BINARY_DIR}/../
  LIBRARY DESTINATION /usr/lib 
  ARCHIVE DESTINATION /usr/lib)

#Copy/install files to a certain static directory
install (FILES ${PROJECT_BINARY_DIR}/../scripts/read_core_usage.py DESTINATION /opt)
