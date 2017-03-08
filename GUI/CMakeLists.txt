cmake_minimum_required(VERSION 2.8)
project(GUI_INT)

find_package(OpenCV REQUIRED )
find_package(OpenGL REQUIRED )

set(OpenGL_INCLUDE_DIR /System/Library/Frameworks/OpenGL.framework/Headers)
set(OpenGL_LIBRARY_DIR /System/Library/Frameworks/OpenGL.framework/Libraries)

include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${OpenGL_INCLUDE_DIR})

link_directories(${OpenCV_LIBRARY_DIR})
link_directories(${OpenGL_LIBRARY_DIR})
include_directories(/usr/local/cuda/include)

add_subdirectory(AR_withGui)
