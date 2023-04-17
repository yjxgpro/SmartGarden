#include <servo.h>

Servo::Servo()
{
    this->servoInit(GPIO_FAN1);
    this->servoInit(GPIO_FAN2);
}

int* Servo:: theta(int _coordinate[2])
{
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
int Servo::pwmPeriod(int _theta)
{
     int period;
        // Map 0 to 180 degrees onto 2 to 12% of duty cycle, hence the period
     period = round((_theta + 45) / 0.09);

        return period;
}

 // Set pin mode to output
 void Servo::servoInit(int pin){
    gpioSetMode(pin, PI_OUTPUT);
        // Send 1500us signal to servo
        gpioServo(pin, 1500);
        time_sleep(0.5);
 }

  void Servo::fanAct(int ENB, int IN3, int IN4) {
        // Set pin mode and values
        gpioSetMode(ENB, PI_OUTPUT);
        gpioSetMode(IN3, PI_OUTPUT);
        gpioSetMode(IN4, PI_OUTPUT);
        gpioWrite(ENB, 1);
        gpioWrite(IN3, 1);
        gpioWrite(IN4, 0);
    }
    
     void Servo::servoAct1(int gpioPin, double x, double y) {
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

       void Servo::servoAct2(int gpioPin, double x, double y) {
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

      void Servo::start(double x, double y){
        thread1 = thread(&Servo::servoAct1, this, GPIO_FAN1, x, y);
        thread2 = thread(&Servo::servoAct2, this, GPIO_FAN2, x, y);
    }

     void Servo::stop(){
        thread1.join();
        thread2.join();
        gpioWrite(GPIO_FAN_EN, 0);
    }
