cmake_minimum_required(VERSION 2.8.11)

project(qt_gui_opencv)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
set(Qt5Core_DIR /Users/DavidWang/Qt/5.7/clang_64/lib/cmake/Qt5Core)
set(Qt5Gui_DIR /Users/DavidWang/Qt/5.7/clang_64/lib/cmake/Qt5Gui)
# set(Qt5Widgets_DIR /Users/DavidWang/Qt/5.7/clang_64/lib/cmake/Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)
# find_package(Qt5Widgets)
find_package(OpenCV)

# Tell CMake to create the helloworld executable
add_executable(test_qt  mainwindow.cpp)

# Use the Widgets module from Qt 5.
target_link_libraries(test_qt Qt5::Core Qt5::Gui ${OpenCV_LIBS})