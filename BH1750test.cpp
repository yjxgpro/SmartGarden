#include "BH1750.h"

using namespace std;

class LEDcallback : public BH1750callback
{
    public:
	 void hasSample(float lightvalue)
	 {
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
    LEDcallback LED1;
     bh1750test.setCallback(&LED1);
     bh1750test.setworkmode(Continue_H_M2); 
     bh1750test.start();
     do {
	 time_sleep(1);
     } while (getchar() < 2);
     bh1750test.stop();
     exit(EXIT_SUCCESS);
}