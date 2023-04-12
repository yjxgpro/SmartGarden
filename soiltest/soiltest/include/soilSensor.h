#ifndef SOILSENSOR_H
#define SOILSENSOR_H

#include <stdio.h>
#include <stdint.h>
#include <thread>
#include <iostream>
using namespace std;

class SoilSensor 
{
    private:
    int m_gpioPin;
    thread* soil_Thread = NULL;
    
    public:
    SoilSensor(int gpioPin);
    ~SoilSensor();

    static void start(SoilSensor* soilsensor);
    void soilthread_run();
    void soilthread_stop();

    static void gpioISR(int gpio, int level, uint32_t tick, void *userData);


};

#endif

