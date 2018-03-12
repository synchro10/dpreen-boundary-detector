cmake_minimum_required(VERSION 3.9)
project(dpreen_segmentation)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)

add_executable(dpreen_segmentation Sources/main.cpp Sources/OpponentColorStage/interface/OpponentColorStage.cpp Sources/OpponentColorStage/interface/OpponentColorStage.h Sources/OpponentColorStage/OCS.cpp Sources/OpponentColorStage/OCS.h Sources/ChromaticContourStage/ChromaticContourStage.cpp Sources/ChromaticContourStage/ChromaticContourStage.h Sources/CompetitiveCooperativeStage/CompetitiveCooperativeStage.cpp Sources/CompetitiveCooperativeStage/CompetitiveCooperativeStage.h)
target_link_libraries( dpreen_segmentation ${OpenCV_LIBS} )