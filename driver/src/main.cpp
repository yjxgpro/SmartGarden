#include "soilSensor.h"
#include <chrono>
#include "camera.h"
#include "servo.h"
#include "BH1750.h"

class ServoSampleCallback : public ServoCallback 
{
    private:
        Servo servo;
        
    void hasSample(int n1, int n2) {
        
        if (n1 > 0 && n2 > 0){
            servo.start(n1, n2);
            servo.fanAct(17, 22, 27);
            servo.stop();
        }
    }
};

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

int main()
{
    SoilSensor sensor(GPIO_SOILSENSOR);
    //BH1750 bh1750test;
    //LEDcallback LED1;
    sensor.soilthread_start();
    Camera camera;
    ServoSampleCallback servoSampleCallback;
    camera.registerCallback(&servoSampleCallback);
    //bh1750test.setCallback(&LED1);
    //bh1750test.setworkmode(Continue_H_M2); 
    //bh1750test.start();
    camera.detect();
    //while(1)
    //{
    //}
    sensor.soilthread_stop();
    bh1750test.stop();
    return 0;
}