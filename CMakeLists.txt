cmake_minimum_required(VERSION 2.8)

add_subdirectory(facematch)


option(DOWNLOAD_DATA "Download dataset 'Labeled Faces in the Wild' [~173Mb] and uncompress it into data directory" OFF)

if(DOWNLOAD_DATA)
    # We are using the "Labeled Faces in the Wild" dataset (http://vis-www.cs.umass.edu/lfw/)
    message(STATUS "Downloading 'Labeled Faces in the Wild' dataset...")
    file(DOWNLOAD http://vis-www.cs.umass.edu/lfw/lfw.tgz ${CMAKE_BINARY_DIR}/data/lfw.tgz
         SHOW_PROGRESS
         EXPECTED_MD5 ac79dc88658530a91423ebbba2b07bf3)
    message(STATUS "Uncompressing dataset...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xvzf ${CMAKE_BINARY_DIR}/data/lfw.tgz
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/data)
endif()
