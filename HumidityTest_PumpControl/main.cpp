#include "Humidity_Motor.h"
#include "Drivers.h"

int main(int argc, char const *argv[])
{
    wiringpisetup_check();
    Humidity_Motor_SignalPin_Setup();
    while(1)
	{
		HumidityTest_Water();
	}
    return 0;
}