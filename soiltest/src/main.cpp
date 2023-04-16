#include "soilSensor.h"
#include <chrono>
#define GPIO_SOILSENSOR 18

int main()
{
    SoilSensor sensor(GPIO_SOILSENSOR);
    sensor.soilthread_start();
    while(1)
    {
    }
    return 0;
}

