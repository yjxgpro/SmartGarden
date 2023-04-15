#include "camera.h"
#include "Servo_new.cpp"

class ServoSampleCallback : public ServoCallback {
    void hasSample(int n1, int n2) {
        Servo servo;
        
        if (n1 > 0 && n2 > 0){
            servo.start(n1, n2);
            servo.fanAct(4, 27, 22);
            servo.stop();
        }
    }
    
//     void hasSample(int n1, int n2) {
//         if (n1 > 0 && n2 > 0)
//         {
//             cout << "pest detected" << endl;
//             cout << "(" << n1 << "," << n2 << ")" << endl;
//         }
//     }
};

int main(int argc, char* argv[]) {
    if(gpioInitialise()<0)
    {
        return 1;
    }
    Camera camera;
    Servo servo;
    servo.servoInit(servo.gpioPin1);
    servo.servoInit(servo.gpioPin2);
    ServoSampleCallback servoSampleCallback;
    camera.registerCallback(&servoSampleCallback);
    camera.detect();
    return 0;
}
