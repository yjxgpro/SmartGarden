/******************************************************************************
Author: Shuhan Dai, 2742227d@student.gla.ac.uk
This file prototypes the SoilSensor class, implemented in soilSensor.cpp.  
Soil moisture detection and automatic watering is a part function of Project "SmartGarden",
please click https://github.com/yjxgpro/SmartGarden.git see more.
******************************************************************************/

#ifndef _SOILSENSOR_H
#define _SOILSENSOR_H

#include <stdio.h>
#include <stdint.h>
#include <thread>
#include <iostream>
using namespace std;

#define GPIO_SOILSENSOR 18


class SoilSensor 
{
    private:
    int m_gpioPin;
    thread* soil_Thread = NULL;
    
    public:
    // Passing the detection pin when creating the SoilSensor class
    SoilSensor(int gpioPin);
    ~SoilSensor();

    static void run(SoilSensor* soilsensor);
    void soilthread_start();
    void soilthread_stop();
    // register callback funtion
    static void soilcallback(int gpio, int level, uint32_t tick, void *userData);


};

#endif
