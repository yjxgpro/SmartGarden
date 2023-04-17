#include "soilSensor.h"
#include <chrono>
#include "camera.h"
#include "Servo_new.cpp"
// #include "pestDetect.cpp"
#define GPIO_SOILSENSOR 18


class ServoSampleCallback : public ServoCallback {
    void hasSample(int n1, int n2) {
        Servo servo;
        
        if (n1 > 0 && n2 > 0){
            servo.start(n1, n2);
            servo.fanAct(GPIO_FAN_EN, GPIO_IN3, GPIO_IN4);
            servo.stop();
        }
    }
    
};

int main()
{
    SoilSensor sensor(GPIO_SOILSENSOR);
    sensor.soilthread_start();
    // if(gpioInitialise()<0)
    // {
    //     return 1;
    // }
    Camera camera;
    Servo servo;
    servo.servoInit(servo.gpioPin1);
    servo.servoInit(servo.gpioPin2);
    ServoSampleCallback servoSampleCallback;
    camera.registerCallback(&servoSampleCallback);
    camera.detect();
    return 0;  
    while(1)
    {
    }
    return 0;
}

