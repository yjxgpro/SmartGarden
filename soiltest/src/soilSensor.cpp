
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

void SoilSensor::start()
{
    if (gpioInitialise() < 0) {
	cout << "fail" << endl;
        return;
    }

    gpioSetMode(m_gpioPin, PI_INPUT);
    gpioSetISRFuncEx(m_gpioPin, RISING_EDGE, ISR_TIMEOUT, gpioISR, (void*)this);
}

void SoilSensor::gpioISR(int gpio, int level, uint32_t tick, void *userData)
{
    SoilSensor* sensor = static_cast<SoilSensor*>(userData);

    if (level == 0) {
        cout << "soil is wet" << endl;
    }
    else {
        cout << "soil is dry" << endl;
    }
}

