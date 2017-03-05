cmake_minimum_required(VERSION 3.6)
project(testlane2)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)
add_executable(testlane2 ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(testlane2 ${OpenCV_LIBS})