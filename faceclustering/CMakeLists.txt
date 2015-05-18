cmake_minimum_required(VERSION 2.8)
project( faceclustering )

find_package( OpenCV REQUIRED )

file(GLOB _headers "*.h")
file(GLOB _src "*.cpp")
set(SRC_FILES
            ${_headers}
			${_src}
			CMakeLists.txt
            )
# Clustering library
find_path(Eigen2_INCLUDE_DIR
  NAMES Eigen/Core
  PATH_SUFFIXES eigen2
)
file(GLOB clustering_headers "clustering/*.h")
file(GLOB clustering_src "clustering/*.cpp")

include_directories(${OpenCV_INCLUDE_DIRS} ${Eigen2_INCLUDE_DIR})
add_executable( faceclustering ${SRC_FILES} ${clustering_src} ${clustering_headers} )
set_property(TARGET faceclustering PROPERTY CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
target_link_libraries( faceclustering ${OpenCV_LIBS} )

# Copy cascade files to build directory.
if(WIN32)
    file(COPY "${OpenCV_CONFIG_PATH}/etc/haarcascades" DESTINATION "${CMAKE_BINARY_DIR}/data/")
elseif(UNIX)
    string(TOLOWER "${OpenCV_CONFIG_PATH}/haarcascades" HAARCASCADES)
    file(COPY "${HAARCASCADES}" DESTINATION "${CMAKE_BINARY_DIR}/data/")
else()
endif()

