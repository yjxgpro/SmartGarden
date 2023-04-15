cmake_minimum_required(VERSION 3.5)

project(MyProject)

# Find required packages
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_library(PIGPIO_LIB pigpio REQUIRED)

# Add executable
add_executable(MyExecutable 
    ServoGetSample.cpp
    camera.h
    pestDetect.cpp
    Servo_new.cpp
)

# Add include directories
target_include_directories(MyExecutable PRIVATE
    # Add any necessary include directories here
    ${OpenCV_INCLUDE_DIRS}
)

# Add any necessary libraries here
target_link_libraries(MyExecutable PRIVATE
    # Add any necessary libraries here
    Threads::Threads
    ${OpenCV_LIBS}
    ${PIGPIO_LIB}
)
