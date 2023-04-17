
#include <iostream>
#include "soilSensor.h"
#include <pigpio.h>
#include <unistd.h>
using namespace std;

#define ENA 21
#define IN1 20
#define IN2 16

#define ISR_TIMEOUT 100000

SoilSensor::SoilSensor(int gpioPin)
{
    m_gpioPin = gpioPin;
}

SoilSensor::~SoilSensor()
{
    gpioTerminate();
}

void SoilSensor::run(SoilSensor* soilsensor)
{
    if (gpioInitialise() < 0)
    {
        cout << "fail" << endl;
    }
    cout << "success" << endl;
    gpioSetMode(soilsensor->m_gpioPin, PI_INPUT);
    gpioSetISRFuncEx(soilsensor->m_gpioPin, RISING_EDGE, ISR_TIMEOUT, soilcallback, soilsensor);
}

void SoilSensor::soilthread_start()
{
    if (soil_Thread)
    {
        cout << "thread already running" << endl;
    }
    else
    {
        soil_Thread = new thread(run, this);
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

void SoilSensor::soilcallback(int gpio, int level, uint32_t tick, void* userData)
{
    SoilSensor *sensor = static_cast<SoilSensor *>(userData);
    gpioSetMode(ENA, PI_OUTPUT);
    gpioSetMode(IN1, PI_OUTPUT);
    gpioSetMode(IN2, PI_OUTPUT);
    gpioWrite(ENA, 0);
    gpioWrite(IN1, 1);
    gpioWrite(IN2, 0);

    if (level == 1)
    {
        sleep(1);
        gpioWrite(ENA, 1);
        sleep(5);
        gpioWrite(ENA, 0);
	    sleep(2);
    }
}
