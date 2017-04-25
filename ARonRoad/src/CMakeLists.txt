add_executable(ARonRoad
main.cpp
        ARDrawingContext.cpp
        CameraCalibration.cpp
        GeometryTypes.cpp

)

target_link_libraries( ARonRoad ${OpenCV_LIBRARIES} )
target_link_libraries( ARonRoad ${OPENGL_LIBRARIES} )
