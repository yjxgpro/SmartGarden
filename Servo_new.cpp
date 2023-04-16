#include <iostream>
#include <cmath>
#include <pigpio.h>
#include <thread>

using namespace std;

#define PI 3.14159265

/* 
    This is the driver for the pan-tilt pest tracking platform, part of the Smart Garden project.
    The driver maps 2-D coordinates from the camera into pitch and yaw angles, and calculates the
    corresponding PWM period to drive the servos. It also activates the fan at the same time.
*/

// Driver class for two servos and a DC fan motor.
class Servo {

public:
    Servo(){}
    // Initiate the threads for two servos
    thread thread1;
    thread thread2;
    
    // Define the pigpio pinout for each component
    int gpioPinFan = 4; // Enable signal for fan motor, set 1 for on, 0 for off
    int gpioIN3 = 27; // VDD pin for fan motor, keep at 1
    int gpioIN4 = 22; // GND pin for fan motor, keep at 0
    int gpioPin1 = 17; // PWM control signal for yaw servo
    int gpioPin2 = 18; // PWM control signal for pitch servo

    /* Member function to servo angles based on given 2-D coordinates. A few assumption are made
    since the coordinates are only 2-D (depth info is missing): 
    
    1. The bug only appears on a 2-D plane 30 cm away from the pan-tilt.
    2. The camera has an FOV of a rectangle that is 40 cm wide and 30 cm high.
    */
    int* theta(int _coordinate[2]) {
        
        // Constants related to distance away from the pan-tilt and FOV of the camera
        int vertical = 30; // Camera horizontal FOV
        int horizontal = 40; // Camera vertical FOV
        int distance = 30; // Distance away from the pan-tilt (at which the bug appears)
        double theta_x, theta_y; 
        static int out[2];

        // Calculate the absolute anlgular position of servos based on input coordinates
        theta_x = 90 - (atan((_coordinate[0]/15 - 0.5 * horizontal) / distance) * 180 / PI);
        theta_y = 90 + (atan((_coordinate[1]*3/48 - 0.5 * vertical) / distance) * 180 / PI);
        
        // Round angles to integer and return as an array
        out[0] = round(theta_x);
        out[1] = round(theta_y);

        return out;
    }
    
    // Member function to calculate PWM period based on angle
    int pwmPeriod(int _theta) {

        int period;
        // Map 0 to 180 degrees onto 2 to 12% of duty cycle, hence the period
        period = round((_theta + 45) / 0.09);

        return period;
    }
    
    /* Member function to initiate both servos at 90 degrees position
    Argument: Servo's PWM pin
    */
    void servoInit(int pin) {
        // Set pin mode to output
        gpioSetMode(pin, PI_OUTPUT);
        // Send 1500us signal to servo
        gpioServo(pin, 1500);
        time_sleep(0.5);
    }
    
    /* Member funciton to initiate and star the fan
    Argument 1: Enable pin
    Argument 2: Input power pin
    Argument 3: GND pin
    */
    void fanAct(int ENB, int IN3, int IN4) {
        // Set pin mode and values
        gpioSetMode(ENB, PI_OUTPUT);
        gpioSetMode(IN3, PI_OUTPUT);
        gpioSetMode(IN4, PI_OUTPUT);
        gpioWrite(ENB, 1);
        gpioWrite(IN3, 1);
        gpioWrite(IN4, 0);
    }
    
    /* Member function to drive the yaw servo based on x-coordinate. 
    Argument 1: pin number for the servo's PWM input
    Argument 2: x-coordinate
    Argument 3: y-coordinate
    */
    void servoAct1(int gpioPin, double x, double y) {
        // Create an array to hold coordinates
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        // Calculate required angles
        int* anglePtr = theta(coordinates);
        // Set PWM period based on x-coordinate
        gpioServo(gpioPin, pwmPeriod(anglePtr[0]));
        time_sleep(0.02);
        // Stop the servo
        gpioServo(gpioPin, 0);
    }
    
    /* Member function to drive the yaw servo based on y-coordinate. 
    Argument 1: pin number for the servo's PWM input
    Argument 2: x-coordinate
    Argument 3: y-coordinate
    */
    void servoAct2(int gpioPin, double x, double y) {
        // Create an array to hold coordinates
        int coordinates[2];
        coordinates[0] = x;
        coordinates[1] = y;
        // Calculate required angles
        int* anglePtr = theta(coordinates);
        // Set PWM period based on y-coordinates
        gpioServo(gpioPin, pwmPeriod(anglePtr[1]));
        time_sleep(0.02);
        // Stop the servo
        gpioServo(gpioPin, 0);
    }
    
    /* Member function to create the threads and activate the servos
    Argument 1: x-coordinate
    Argument 2: y-coordinate
    */
    void start(double x, double y){
        thread1 = thread(&Servo::servoAct1, this, gpioPin1, x, y);
        thread2 = thread(&Servo::servoAct2, this, gpioPin2, x, y);
    }
    
    // Member function to kill the servo threads and stop the fan    
    void stop(){
        thread1.join();
        thread2.join();
        gpioWrite(gpioPinFan, 0);
    }

};


