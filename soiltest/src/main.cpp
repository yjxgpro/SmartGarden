#include "soilSensor.h"

int main()
{
    SoilSensor sensor(17);
    sensor.start();

    // Wait for sensor input
    while (1) {}

    return 0;
}

