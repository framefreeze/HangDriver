cmake_minimum_required(VERSION 2.8)
project(back_detection_cuda)

#set(CMAKE_PREFIX_PATH /home/framefreeze/diy_install/opencv/share/OpenCV)
set(cuda_include /usr/local/cuda/include)
message(${cuda_include})

find_package(OpenCV 3.2.0 REQUIRED)
find_package(OpenGL REQUIRED)
#set(CMAKE_CXX_STANDARD 11)
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(/usr/local/cuda/include)

# compile
add_subdirectory(AR)
#set(SOURCE_FILES main.cpp)
#add_executable(back_detection ${SOURCE_FILES})

