#include <iostream>
#include <cmath>
#include <wiringPi.h>
#include <thread>
using namespace std;

#define PI 3.14159265

class Servo {

public:
    
    int x = n1;
    int y = n2;
    int pwmPin1 = 0;
    int pwmPin2 = 2;
    int* theta(int _coordinate[2]) {

        int vertical = 60;
        int horizontal = 60;
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

    void servoInit(int pin1, int pin2) {
        int counter = 30;
        int counter2 = 30;
        wiringPiSetup();
        pinMode(pwmPin1, OUTPUT);
        pinMode(pwmPin2, OUTPUT);

        while (counter--) {
            digitalWrite(pin1, HIGH);
            delayMicroseconds(1500);
            digitalWrite(pin1, LOW);
            delayMicroseconds(20000 - 1500);
        }

        while (counter2--) {
            digitalWrite(pin2, HIGH);
            delayMicroseconds(1500);
            digitalWrite(pin2, LOW);
            delayMicroseconds(20000 - 1500);
        }
    }
    void static servoAct1(double x, double y) {
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        int* anglePtr = theta(coordinates);
        digitalWrite(0, HIGH);
        delayMicroseconds(pwmPeriod(anglePtr[0]));
        digitalWrite(0, LOW);
        delayMicroseconds(20000 - pwmPeriod(anglePtr[0]));
    }

    void servoAct2(double x, double y) {
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        int* anglePtr = theta(coordinates);
        digitalWrite(2, HIGH);
        delayMicroseconds(pwmPeriod(anglePtr[1]));
        digitalWrite(2, LOW);
        delayMicroseconds(20000 - pwmPeriod(anglePtr[1]));
    }

    void servoActivate(int x,int y) {
        
        servoInit(0,2);
        while (1) {
            servoAct1(x,y)
            //thread th1(servoAct1, k, 60-k);
            //thread th2(servoAct2, k, 60-k);
           
            delay(100);

        }
        
    }

};

