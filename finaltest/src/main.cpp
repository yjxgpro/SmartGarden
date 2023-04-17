#include "soilSensor.h"
#include <chrono>
#include "camera.h"
#include "servo.h"

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


int main()
{
    SoilSensor sensor(GPIO_SOILSENSOR);
    sensor.soilthread_start();
    Camera camera;
    ServoSampleCallback servoSampleCallback;
    camera.registerCallback(&servoSampleCallback);
    camera.detect();
    //while(1)
    //{
    //}
    sensor.soilthread_stop();
    return 0;
}
