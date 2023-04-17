cmake_minimum_required(VERSION 3.5)

project(smartgarden)

# Find required packages
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_library(PIGPIO_LIB pigpio REQUIRED)
find_package(Qt5 COMPONENTS Core Network REQUIRED)


include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add executable
add_executable(smartgarden 
    src/main.cpp
    src/pestDetect.cpp
    src/servo.cpp
    src/soilSensor.cpp
    src/BH1750.cpp
)

# Add include directories
target_include_directories(smartgarden PRIVATE
    # Add any necessary include directories here
    ${OpenCV_INCLUDE_DIRS}
)

# Add any necessary libraries here
target_link_libraries(smartgarden PRIVATE
    # Add any necessary libraries here
    Threads::Threads
    ${OpenCV_LIBS}
    ${PIGPIO_LIB} 
    Qt5::Core Qt5::Network
)
