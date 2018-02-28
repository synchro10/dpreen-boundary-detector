cmake_minimum_required(VERSION 3.9)
project(dpreen_segmentation)
find_package( OpenCV REQUIRED )
set(CMAKE_CXX_STANDARD 17)

add_executable(dpreen_segmentation Sources/main.cpp Sources/OpponentColorStage/OpponentColorStage.cpp Sources/OpponentColorStage/OpponentColorStage.h)
target_link_libraries( dpreen_segmentation ${OpenCV_LIBS} )