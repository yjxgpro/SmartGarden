#include "Humidity_Motor.h"
#include "Drivers.h"


void II2Ccheck(void)
{
     // todo 检测II2C是否正常
}

/*wiringpi初始化检测*/
void wiringpisetup_check(void)
{
    if (wiringPiSetup() == -1)
    {
        exit(1);
    }
}

/*湿度传感器/马达信号脚初始化*/
void Humidity_Motor_SignalPin_Setup(void)
{
    button = 0;
    pinMode(MotorSignalPin, OUTPUT);
    pinMode(HumiditySignalPin, INPUT);
    digitalWrite(HumiditySignalPin, 0); //Wet状态
    digitalWrite(MotorSignalPin, 0);   //Motor关毕状态
}

/*湿度检测，马达控制浇水*/
void HumidityTest_Water(void)
{
    if (digitalRead(HumiditySignalPin) == Dry)
        {
			cout << "dry"<< endl;
            digitalWrite(MotorSignalPin, 1);
		}
		else
        {
            if(button)
            {
              digitalWrite(MotorSignalPin, 1);// 
              delay(500);
              digitalWrite(MotorSignalPin, 0); //目前处理方式是按一下button放一会水motor自动停 
              button = 0;
            }
            else
            {
                cout << "wet"<< endl;
                digitalWrite(MotorSignalPin, 0);
            };
		}
		delay(1000);
}

/*湿度数据读取*/
float HumidityDataRead(void)
{
                    // 直接读取引脚模拟值？
    return 6.6;     // todo 目前还不确定能否读出湿度温度准确值，若可以,需要记录并传给HumiditySensor对象用于后续QT用户读取
}