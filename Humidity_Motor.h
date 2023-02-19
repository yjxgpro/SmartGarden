#ifndef HumiditySignalPin
 #define HumiditySignalPin 0
#endif
#ifndef MotorSignalPin 
 #define MotorSignalPin 1
#endif
#define Dry 1
#define Wet 0
extern bool button = 0;

void Humidity_Motor_SignalPin_Setup(void);
void HumidityTest_Water(void);
float HumidityDataRead(void);
void II2Ccheck(void);

class HumiditySenor
{
    public: 
    prviate:
};

class Motor
{
    public: 
    bool button;    //用户可以在QT界面手动打开马达
    prviate:
};
 Motor Watermotor;