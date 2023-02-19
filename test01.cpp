#include <iostream>
#include <wiringPi.h>
#define PIN0 0
#define PIN1 1
using namespace std;

int main(int argc, char const *argv[])
{
    if (wiringPiSetup() == -1)
    {
        exit(1);
    }

    pinMode(PIN1, OUTPUT);
    pinMode(PIN0, INPUT);

    while(1)
	{
		if (digitalRead(PIN0) == HIGH)
        {
			cout << "dry"<< endl;
            digitalWrite(PIN1, 1);
		}
		else
        {
			cout << "wet"<< endl;
            digitalWrite(PIN1, 0);
		}
		delay(1000);
	}
    return 0;
}

