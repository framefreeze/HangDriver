add_executable(ARonRoad
main.cpp
)

target_link_libraries( ARonRoad ${OpenCV_LIBRARIES} )
target_link_libraries( ARonRoad ${OPENGL_LIBRARIES} )
