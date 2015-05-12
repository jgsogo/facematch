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
target_link_libraries( facematch ${OpenCV_LIBS} )

# Copy cascade files to build directory.
file(COPY "${OpenCV_CONFIG_PATH}/etc" DESTINATION "${CMAKE_BINARY_DIR}/data")
