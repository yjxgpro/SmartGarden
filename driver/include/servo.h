/* 
    Author: Anthony Jin, 2789782J@student.gla.ac.uk
    
    This is the driver for the pan-tilt pest tracking platform, part of the Smart Garden project.
    The driver maps 2-D coordinates from the camera into pitch and yaw angles, and calculates the
    corresponding PWM period to drive the servos. It also activates the fan motor at the same time.
    
    Please refer to https://github.com/yjxgpro/SmartGarden for the full project and source code.
*/

#include <iostream>
#include <cmath>
#include <pigpio.h>
#include <thread>

using namespace std;

#define PI 3.14159265

#define GPIO_FAN_EN 17 // Enable signal for fan motor, set 1 for on, 0 for off
#define GPIO_IN3 22 // VDD pin for fan motor, keep at 1
#define GPIO_IN4 27 // GND pin for fan motor, keep at 0
#define GPIO_FAN1 23 // PWM control signal for yaw servo
#define GPIO_FAN2 24 // PWM control signal for pitch servo

// Driver class for two servos and a DC fan motor.
class Servo {

public:
    Servo();
    // Initiate the threads for two servos
    thread thread1;
    thread thread2;

    /* Member function to calculate servo angles based on given 2-D coordinates. A few assumption are made
    since the coordinates are only 2-D (depth info is missing): 
    
    1. The bug only appears on a 2-D plane 30 cm away from the pan-tilt.
    2. The camera has an FOV of a rectangle that is 40 cm wide and 30 cm high.
    */
    int* theta(int * _coordinate);

    
    // Member function to calculate PWM period based on angle
    int pwmPeriod(int _theta);

    /* Member function to initiate both servos at 90 degrees position
    Argument: Servo's PWM pin
    */
    void servoInit(int pin);
    
    /* Member funciton to initiate and star the fan
    Argument 1: Enable pin
    Argument 2: Input power pin
    Argument 3: GND pin
    */
    void fanAct(int ENB, int IN3, int IN4);
    
    /* Member function to drive the yaw servo based on x-coordinate. 
    Argument 1: pin number for the servo's PWM input
    Argument 2: x-coordinate
    Argument 3: y-coordinate
    */
    void servoAct1(int gpioPin, double x, double y) ;
    
    /* Member function to drive the yaw servo based on y-coordinate. 
    Argument 1: pin number for the servo's PWM input
    Argument 2: x-coordinate
    Argument 3: y-coordinate
    */
    void servoAct2(int gpioPin, double x, double y) ;
    
    /* Member function to create the threads and activate the servos
    Argument 1: x-coordinate
    Argument 2: y-coordinate
    */
    void start(double x, double y);
    
    // Member function to kill the servo threads and stop the fan    
    void stop();

};

