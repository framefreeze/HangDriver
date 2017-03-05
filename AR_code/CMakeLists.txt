cmake_minimum_required(VERSION 2.8)
project(MARKERLESS_AR)

find_package(OpenCV REQUIRED )
find_package(OpenGL REQUIRED )

#set(OpenGL_INCLUDE_DIR /System/Library/Frameworks/OpenGL.framework/Headers)
#set(OpenGL_LIBRARY_DIR /System/Library/Frameworks/OpenGL.framework/Libraries)

#message(${OpenGL_INCLUDE_DIR})

include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${OpenGL_INCLUDE_DIR})

link_directories(${OpenCV_LIBRARY_DIR})
link_directories(${OpenGL_LIBRARY_DIR})

include_directories(${MARKERLESS_AR_SOURCE_DIR}/src)
add_subdirectory(src)
