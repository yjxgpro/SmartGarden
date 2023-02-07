#include <iostream>
#include <wiringPi.h>

#define PIN_0 0
using namespace std;

int main(int argc, char **argv)
{
	wiringPiSetup();
	pinMode(PIN_0, INPUT);
	
	while(1)
	{
		if (digitalRead(PIN_0) == HIGH){
			cout << "dry"<< endl;
			}
		else {
			cout << "wet"<< endl;
			}
		delay(2000);
		}
	
	return 0;
}
