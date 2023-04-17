cmake_minimum_required(VERSION 3.0)

project(soilSensor)

set(CMAKE_CXX_STANDARD 11)

# Set the source files
set(SOURCES 
    src/soilSensor.cpp
    src/main.cpp
)

# Add include directories
include_directories(include)

# Add pigpio library
link_directories(/usr/local/lib)
link_libraries(pigpio pthread)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

