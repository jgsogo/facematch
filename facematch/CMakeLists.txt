cmake_minimum_required(VERSION 2.8)
project( facematch )

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( facematch main.cpp facematch.h facematch.cpp )
target_link_libraries( facematch ${OpenCV_LIBS} )
