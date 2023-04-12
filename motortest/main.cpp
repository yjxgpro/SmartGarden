#include <pigpio.h>
#include <iostream>
#include <unistd.h>

#define ENB 18
#define IN3 27
#define IN4 22

int main(int argc, char const *argv[])
{
    if (gpioInitialise() < 0)
        return -1;

    gpioSetMode(ENB, PI_OUTPUT);
    gpioSetMode(IN3, PI_OUTPUT);
    gpioSetMode(IN4, PI_OUTPUT);

    gpioWrite(ENB, 0);
    gpioWrite(IN3, 0);
    gpioWrite(IN4, 0);

    sleep(2);

    gpioWrite(ENB, 1);
    gpioWrite(IN3, 1);

    sleep(10);
    gpioWrite(IN3, 0);

    gpioTerminate();
    return 0;
}
