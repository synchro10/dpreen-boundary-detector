cmake_minimum_required(VERSION 3.9)
project(dpreen_segmentation)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)

add_executable(dpreen_segmentation Sources/main.cpp Sources/OpponentColorStage/OpponentColorStage.cpp Sources/OpponentColorStage/OpponentColorStage.h Sources/ChromaticContourStage/ChromaticContourStage.cpp Sources/ChromaticContourStage/ChromaticContourStage.h Sources/CompetitiveCooperativeStage/CompetitiveCooperativeStage.cpp Sources/CompetitiveCooperativeStage/CompetitiveCooperativeStage.h Sources/Model/IModel.h Sources/Model/Model.cpp Sources/Model/Model.h Sources/Utility/Util.cpp Sources/Utility/Util.h Sources/GlobalConstants.h Sources/RegionEnhancementStage/RegionEnhancementStage.cpp Sources/RegionEnhancementStage/RegionEnhancementStage.h)
target_link_libraries( dpreen_segmentation ${OpenCV_LIBS} )