#!/bin/zsh 
# 自动编译脚本
mkdir release
cd release
cmake -DOpenGL_INCLUDE_DIR=/System/Library/Frameworks/OpenGL.framework/Headers \
-DOpenGL_LIBRARY_DIR=/System/Library/Frameworks/OpenGL.framework/Libraries ..
make 
 