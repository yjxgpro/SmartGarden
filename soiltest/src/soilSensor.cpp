
#include <iostream>
#include "soilSensor.h"
#include <pigpio.h>
using namespace std;

#define ISR_TIMEOUT 1000

SoilSensor::SoilSensor(int gpioPin)
{
    m_gpioPin = gpioPin;
}

SoilSensor::~SoilSensor()
{
    gpioTerminate();
}

void SoilSensor::start(SoilSensor* soilsensor)
{
    if (gpioInitialise() < 0)
    {
        cout << "fail" << endl;
        return;
    }

    gpioSetMode(soilsensor->m_gpioPin, PI_INPUT);
    gpioSetISRFuncEx(soilsensor->m_gpioPin, RISING_EDGE, ISR_TIMEOUT, gpioISR, soilsensor);
}

void SoilSensor::soilthread_run()
{
    if (soil_Thread)
    {
        cout << "thread already running" << endl;
    }
    else
    {
        soil_Thread = new thread(start, this);
    }
}

void SoilSensor::soilthread_stop()
{
    if(soil_Thread)
    {
        soil_Thread->join();
        delete soil_Thread;
        soil_Thread = NULL;
    }
}

void SoilSensor::gpioISR(int gpio, int level, uint32_t tick, void *userData)
{
    SoilSensor *sensor = static_cast<SoilSensor *>(userData);

    if (level == 1)
    {
        cout << "soil is dry" << endl;
    }
    else
    {
        cout << "soil is fine" << endl;
    }
}
