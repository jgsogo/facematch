cmake_minimum_required(VERSION 2.8)
project( facerecognition )

find_package( OpenCV REQUIRED )

file(GLOB _headers "*.h")
file(GLOB _src "*.cpp")
set(SRC_FILES
            ${_headers}
	    ${_src}
	    CMakeLists.txt
            )

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( facerecognition ${SRC_FILES})
set_property(TARGET facerecognition PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
target_link_libraries( facerecognition ${OpenCV_LIBS} )


