#
# roverapp CMake file
# https://github.com/app4mc-rover/rover-app
#

cmake_minimum_required (VERSION 2.8.11)
project (roverapp)

#For find_package packages, export someextlib_DIR=/path/to/..

# Set directories for external libs

# Set include directories for external libs
set (jsoncpp_INCLUDE_DIRS ${PROJECT_BINARY_DIR}/../dependencies/jsoncpp/include)

# Required packages
find_package (OpenCV REQUIRED)
#find_package (raspicam REQUIRED)
#find_package (wiringPi REQUIRED)
#find_package (Threads)

#Where to put binary files after "make"
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#Where to put library files after "make"
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/libs)

#Include directories
include_directories (${PROJECT_BINARY_DIR}/../include)
include_directories (${PROJECT_BINARY_DIR}/../include/api)
include_directories (${PROJECT_BINARY_DIR}/../include/tasks)
include_directories (${PROJECT_BINARY_DIR}/../include/libraries/hono_interaction/)
include_directories (${PROJECT_BINARY_DIR}/../include/libraries/pthread_distribution_lib/)
include_directories (${PROJECT_BINARY_DIR}/../include/libraries/pthread_monitoring/)
include_directories (${PROJECT_BINARY_DIR}/../include/libraries/status_library/)
include_directories (${PROJECT_BINARY_DIR}/../include/libraries/timing/)
include_directories (${PROJECT_BINARY_DIR}/../include/drivers/oled_drivers)
include_directories (${PROJECT_BINARY_DIR}/../include/drivers/bluetooth_drivers)

#Include directories for external libraries
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${jsoncpp_INCLUDE_DIRS} )

#Add api
add_library(roverapi SHARED ${PROJECT_BINARY_DIR}/../src/api/basic_psys_rover.c)

#Add our custom libraries
add_library (hono_interaction SHARED ${PROJECT_BINARY_DIR}/../src/libraries/hono_interaction/hono_interaction.cpp)
add_library (pthread_distribution SHARED ${PROJECT_BINARY_DIR}/../src/libraries/pthread_distribution_lib/pthread_distribution.cpp)
add_library (status_library SHARED ${PROJECT_BINARY_DIR}/../src/libraries/status_library/status_library.cpp)
add_library (pthread_monitoring SHARED ${PROJECT_BINARY_DIR}/../src/libraries/pthread_monitoring/collect_thread_name.cpp)
add_library (timing SHARED ${PROJECT_BINARY_DIR}/../src/libraries/timing/timing.cpp)

#Add tasks
#Add all files to a variable TASK_FILES
file (GLOB_RECURSE TASK_FILES ${PROJECT_BINARY_DIR}/../src/tasks/*cpp)
add_library (roverapptasks SHARED ${TASK_FILES})

#Add main executable
add_executable(roverapp ${PROJECT_BINARY_DIR}/../src/roverapp.cpp)

#Link external libraries
target_link_libraries (roverapp jsoncpp)

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

#Install binary, shared library, static library (archives)
#install (TARGETS discoverymanager dmLib
#  RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/build
#  LIBRARY DESTINATION /usr/lib 
#  ARCHIVE DESTINATION /usr/lib)
