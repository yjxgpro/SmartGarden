#ifndef SOILSENSOR_H
#define SOILSENSOR_H

#include <stdio.h>
#include <stdint.h>
#include <thread>

class SoilSensor {
public:
    SoilSensor(int gpioPin);
    ~SoilSensor();

    void start();

    static void gpioISR(int gpio, int level, uint32_t tick, void *userData);

private:
    int m_gpioPin;
};

#endif

