#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "BH1750.h"

int main(int argc, char *argv[]) {
    BH1750 bh1750test;
    LEDcallback LED1;
    bh1750test.setCallback(&LED1);
    LED1.currentmode = continue_r2; 
    bh1750test.start();
    do {
	sleep(1);
    } while (getchar() < 10);
    bh1750test.end();
    exit(EXIT_SUCCESS);
}
