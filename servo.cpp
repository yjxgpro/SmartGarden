#include <iostream>
#include <cmath>
#include <wiringPi.h>
using namespace std;

#define PI 3.14159265


int* theta(int _coordinate[2]){
    
    int vertical = 60;
    int horizontal = 60;
    int distance = 30;
    double theta_x, theta_y;
    static int out[2];

    theta_x = 90 - (atan((_coordinate[0]-0.5*horizontal)/distance) *180 / PI);
    theta_y = 90 - (atan((_coordinate[1]-0.5*vertical)/distance) * 180 / PI);
    out[0] = round(theta_x);
    out[1] = round(theta_y);

    return out;
}

int pwmPeriod(int _theta){

    int period;
    period = round((_theta + 45)/0.09);

    return period;
}

void servoInit(){

    int counter = 30;
    int counter2 = 30;

    while(counter--){
        digitalWrite(0,HIGH);
        delayMicroseconds(1500);
        digitalWrite(0,LOW);
        delayMicroseconds(20000 - 1500);
    }

    while(counter2--){
        digitalWrite(2,HIGH);
        delayMicroseconds(1500);
        digitalWrite(2,LOW);
        delayMicroseconds(20000 - 1500);
    }
}

int main(){

    int pwmPin1 = 0;
    int pwmPin2 = 2;   
    wiringPiSetup();
    pinMode(pwmPin1,OUTPUT);
    pinMode(pwmPin2,OUTPUT);
    int coordinates[2];
    int* anglePtr;

    servoInit();

    while(1){

        int counter = 30;
        int counter2 = 30;
        cout << "--------------------" << endl;
        cout << "Enter x coordinate: " << endl;;
        cin >> coordinates[0];
        cout << "Enter y coordinate: " << endl;
        cin >> coordinates[1];
        anglePtr = theta(coordinates);
    
        while (counter--)
        {   
            digitalWrite(0,HIGH);
            delayMicroseconds(pwmPeriod(anglePtr[0]));
            digitalWrite(0,LOW);
            delayMicroseconds(20000 - pwmPeriod(anglePtr[0]));
        }
        while (counter2--)
        {   
            digitalWrite(2,HIGH);
            delayMicroseconds(pwmPeriod(anglePtr[1]));
            digitalWrite(2,LOW);
            delayMicroseconds(20000 - pwmPeriod(anglePtr[1]));
        }
        delay(3000);
        servoInit();
    }
}