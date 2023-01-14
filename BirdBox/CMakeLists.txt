cmake_minimum_required(VERSION 3.24)
project(BirdBox)

set(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#openCV
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(BirdBox main.cpp)

#openCV
target_link_libraries( BirdBox ${OpenCV_LIBS} )