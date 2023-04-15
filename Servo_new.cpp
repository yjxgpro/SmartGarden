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
    //thread thread3;
    int gpioPinFan = 4;
    int gpioIN3 = 27;
    int gpioIN4 = 22;
    int gpioPin1 = 17;
    int gpioPin2 = 18;

    int* theta(int _coordinate[2]) {

        int vertical = 30;
        int horizontal = 40;
        int distance = 30;
        double theta_x, theta_y;
        static int out[2];

        theta_x = 90 - (atan((_coordinate[0]/15 - 0.5 * horizontal) / distance) * 180 / PI);
        theta_y = 90 + (atan((_coordinate[1]*3/48 - 0.5 * vertical) / distance) * 180 / PI);
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

    void fanAct(int ENB, int IN3, int IN4) {
        gpioSetMode(ENB, PI_OUTPUT);
        gpioSetMode(IN3, PI_OUTPUT);
        gpioSetMode(IN4, PI_OUTPUT);
        gpioWrite(ENB, 1);
        gpioWrite(IN3, 1);
        gpioWrite(IN4, 0);
    }

    void servoAct1(int gpioPin, double x, double y) {
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        int* anglePtr = theta(coordinates);
        gpioServo(gpioPin, pwmPeriod(anglePtr[0]));
        time_sleep(0.02);
        gpioServo(gpioPin, 0);
    }
    
    void servoAct2(int gpioPin, double x, double y) {
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        int* anglePtr = theta(coordinates);
        gpioServo(gpioPin, pwmPeriod(anglePtr[1]));
        time_sleep(0.02);
        gpioServo(gpioPin, 0);
    }

    void start(double x, double y){
        thread1 = thread(&Servo::servoAct1, this, gpioPin1, x, y);
        thread2 = thread(&Servo::servoAct2, this, gpioPin2, x, y);
        //thread3 = thread(&Servo::fanAct, this, gpioPinFan, gpioIN3, gpioIN4);
    }

    void stop(){
        thread1.join();
        thread2.join();
        //thread3.join();
        gpioWrite(gpioPinFan, 0);
    }

};

/*
int main(){
    int x;
    int y;
    int k = 0;
    Servo servo;
    gpioInitialise();
    servo.servoInit(servo.gpioPin1);
    servo.servoInit(servo.gpioPin2);
    
    while(k<600){
        
        servo.start(k,600-k);
        k = k + 1;
        //time_sleep(0.1);
        servo.stop();
    }
    
    gpioTerminate();
    return 0;
}

*/
