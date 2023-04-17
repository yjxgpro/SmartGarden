#include "camera.h"
#include "Servo_new.cpp"

class ServoSampleCallback : public ServoCallback {
    private:
        Servo servo;
        
    void hasSample(int n1, int n2) {
        
        if (n1 > 0 && n2 > 0){
            servo.start(n1, n2);
            servo.fanAct(17, 22, 27);
            servo.stop();
        }
    }

int main(int argc, char* argv[]) {
    if(gpioInitialise()<0)
    {
        return 1;
    }
    Camera camera;
    ServoSampleCallback servoSampleCallback;
    camera.registerCallback(&servoSampleCallback);
    camera.detect();
    return 0;
}
