#include "soilSensor.h"
#include <chrono>


void send_message(SoilSensor* sensor)
{
    int count = 0;
    while(true)
    {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "this is message" << count << endl;
        count++;
    }
}

int main()
{
    SoilSensor sensor(17);
    sensor.soilthread_run();
    // Wait for sensor input
    // while (1) {}

    thread thread2(send_message, &sensor);
    thread2.join();

    return 0;
}

