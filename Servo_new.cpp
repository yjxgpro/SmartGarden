#include <iostream>
#include <cmath>
#include <pigpio.h>
#include <thread>

using namespace std;

#define PI 3.14159265

class Servo {

public:
    Servo(){}
    thread thread1;
    thread thread2;
    int gpioPin1 = 17;
    int gpioPin2 = 18;

    int* theta(int _coordinate[2]) {

        int vertical = 600;
        int horizontal = 600;
        int distance = 30;
        double theta_x, theta_y;
        static int out[2];

        theta_x = 90 - (atan((_coordinate[0] - 0.5 * horizontal) / distance) * 180 / PI);
        theta_y = 90 - (atan((_coordinate[1] - 0.5 * vertical) / distance) * 180 / PI);
        out[0] = round(theta_x);
        out[1] = round(theta_y);

        return out;
    }

    int pwmPeriod(int _theta) {

        int period;
        period = round((_theta + 45) / 0.09);

        return period;
    }

    void servoInit(int pin) {
        gpioSetMode(pin, PI_OUTPUT);
        gpioServo(pin, 1500);
        time_sleep(0.5);
    }

    void servoAct(int gpioPin, double x, double y) {
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        int* anglePtr = theta(coordinates);
        gpioServo(gpioPin, pwmPeriod(anglePtr[0]));
        time_sleep(0.02);
        gpioServo(gpioPin, 0);
    }

    void start(double x, double y){
        thread1 = thread(&Servo::servoAct, this, gpioPin1, x, y);
        thread2 = thread(&Servo::servoAct, this, gpioPin2, x, y);
    }

    void stop(){
        thread1.join();
        thread2.join();
    }

};

/*
int main(){
    int x;
    int y;
    Servo servo;
    gpioInitialise();
    servo.servoInit(servo.gpioPin1);
    servo.servoInit(servo.gpioPin2);
    
    while(1){
        cout << "Enter coordinate x: " << endl;
        cin >> x;
        cout << "Enter coordinate y: " << endl;
        cin >> y;
        
        servo.start(x,y);
        time_sleep(0.1);
        servo.stop();
    }
    
    gpioTerminate();
    return 0;
}
*/
