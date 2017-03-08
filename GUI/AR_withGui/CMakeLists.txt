add_executable(markerless_ar ARDrawingContext.cpp
        ARDrawingContext.hpp
        CameraCalibration.cpp
        CameraCalibration.hpp
        GeometryTypes.cpp
        GeometryTypes.hpp
        main.cpp
        ARPipeline.hpp
        ARPipeline.cpp
        Pattern.cpp
        Pattern.hpp
        PatternDetector.cpp
        PatternDetector.hpp
        DebugHelpers.hpp
        hangGUI.cpp
        hangGUI.hpp
        )

target_link_libraries( markerless_ar ${OpenCV_LIBRARIES} )
target_link_libraries( markerless_ar ${OPENGL_LIBRARIES} ${LD_LIBRARY_PATH})
# link
#target_link_libraries(back_detection ${OpenCV_LIBS} ${LD_LIBRARY_PATH})