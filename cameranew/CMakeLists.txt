cmake_minimum_required(VERSION 3.0)

project(camerashow)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Core Network REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(camerashow main.cpp)

target_link_libraries(camerashow ${OpenCV_LIBS} Qt5::Core Qt5::Network)
