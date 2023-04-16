#include "BH1750.h"

using namespace std;

class LEDcallback : public BH1750callback
{
    public:
	 void hasSample(float lightvalue) {
		//根据不同的光照强度，PWM输出
		//test 就可以使用
		float lightneed = 0;
		if(lightvalue>EnoughLight) 
		std::cout<<"Enough Light"<<endl;
		else
		{
			std::cout<<"PWM"<<endl;
			lightneed = EnoughLight-lightvalue;
			int pwmValue = lightneed * pwmRange / 100;
             gpioPWM(LEDgpio, pwmValue); // set PWM 
		}
	}
};

int main(int argc, char *argv[]) {
    BH1750 bh1750test;
	std::cout<<"BH1750 OBJECT success"<<endl;
    LEDcallback LED1;
	std::cout<<"LED OBJECT success"<<endl;
     bh1750test.setCallback(&LED1);
     bh1750test.setworkmode(Continue_H_M2); 
     bh1750test.start();
     do {
	 sleep(1);
     } while (getchar() < 10);
     bh1750test.stop();
     exit(EXIT_SUCCESS);
}