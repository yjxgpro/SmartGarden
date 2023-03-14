#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "BH1750.h"

class LEDcallback : public BH1750callback
{
    public:
	 void hasSample(float lightvalue) {
		//根据不同的光照强度，PWM输出
		//test 就可以使用
		float lightneed;
		if(lightvalue>EnoughLight) 
		std::cout<<"enough light"<<endl;
		else
		{
			std::cout<<"PWM"<<endl;
		// lightneed = EnoughLight - lightvalue;
		// gpioSetMode(LED_control1, PI_OUTPUT);  //  pigpio 控制占空比
        // gpioSetMode(LED_control2, PI_OUTPUT); //  
		// lightvalue=0;
		// lightneed=0;
		}
		// fprintf();// todo 
	}
};

int main(int argc, char *argv[]) {
    BH1750 bh1750test;
	std::cout<<"BH1750 OBJECT success"<<endl;
    LEDcallback LED1;
	std::cout<<"LED OBJECT success"<<endl;
     bh1750test.setCallback(&LED1);
     bh1750test.setworkmode(continue_r2); 
     bh1750test.start();
     do {
	 sleep(1);
     } while (getchar() < 100);
     bh1750test.stop();
     exit(EXIT_SUCCESS);
}