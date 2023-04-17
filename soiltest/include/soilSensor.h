#ifndef _SOILSENSOR_H
#define _SOILSENSOR_H

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

    static void run(SoilSensor* soilsensor);
    void soilthread_start();
    void soilthread_stop();

    static void soilcallback(int gpio, int level, uint32_t tick, void *userData);


};

#endif

