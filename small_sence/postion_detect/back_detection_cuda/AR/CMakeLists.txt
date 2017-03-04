add_executable(markerless_ar ARDrawingContext.cpp
ARDrawingContext.hpp
CameraCalibration.cpp
CameraCalibration.hpp
GeometryTypes.cpp
GeometryTypes.hpp
ARPipeline.hpp
ARPipeline.cpp
Pattern.cpp
Pattern.hpp
PatternDetector.cpp
PatternDetector.hpp
DebugHelpers.hpp
)

target_link_libraries( markerless_ar ${OpenCV_LIBRARIES} )
target_link_libraries( markerless_ar ${OPENGL_LIBRARIES} )
