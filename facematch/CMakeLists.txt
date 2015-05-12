cmake_minimum_required(VERSION 2.8)
project( facematch )

find_package( OpenCV REQUIRED )

file(GLOB _headers "*.h")
file(GLOB _src "*.cpp")
set(SRC_FILES
            ${_headers}
			${_src}
			CMakeLists.txt
            )
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( facematch ${SRC_FILES} )
set_property(TARGET facematch PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
target_link_libraries( facematch ${OpenCV_LIBS} )

# Copy cascade files to build directory.
if(WIN32)
    file(COPY "${OpenCV_CONFIG_PATH}/etc/haarcascades" DESTINATION "${CMAKE_BINARY_DIR}/data/")
elseif(UNIX)
    string(TOLOWER "${OpenCV_CONFIG_PATH}/haarcascades" HAARCASCADES)
    file(COPY "${HAARCASCADES}" DESTINATION "${CMAKE_BINARY_DIR}/data/")
else()
endif()

