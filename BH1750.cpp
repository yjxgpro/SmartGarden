#include "BH1750.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <iostream>
#include "pigpio.h"

using namespace std;
#ifndef NDEBUG
#define DEBUG
#endif

uint8_t BH1750::currentworkmode;
bool BH1750::recflag = 0;

BH1750::BH1750(BH1750DeviceSettings bh1750deviceSettings)
{
    device = bh1750deviceSettings;
#ifdef DEBUG
    fprintf(stderr, "I2c: bus=%02x, BH1750Addr=%02x\n", device.i2c_bus, device.BHAddress);
#endif
    BH1750initgpio();
    BH1750WritePoweron(); // power on   //先写入一个死值，不确定寄存器地址address
    //time_sleep(0.2);
}

void BH1750::BH1750initgpio()
{
    if (device.initPIGPIO)
    {
        int cfg = gpioCfgGetInternals();
        cfg |= PI_CFG_NOSIGHANDLER;
        gpioCfgSetInternals(cfg);
        int r = gpioInitialise();
        if (r < 0)
        {
            char msg[] = "pigpio initialisation failed.";
#ifdef DEBUG
            std::cout << msg << endl;
#endif
            throw msg;
        }
        else
        {
            std::cout << "gpio initialisation success" << endl;
        }
    }
}

void BH1750::start()
{
    if (USThread)
    {
        std::cout << "thread already running";
        return;
    }
    else
    {
        USThread = new std::thread(BH1750::run, this); // run作线程入口，this作传入参数
    }
}

void BH1750::run(BH1750 *BH1750) // 这边有个bug 好像每次今来都要上电 增加耗时？
{
    std::cout << "create thread success" << endl;
    gpioSetTimerFuncEx(Timer0, LightPeriod, reinterpret_cast<gpioTimerFuncEx_t>(BH1750::gpioTimercallFuc), BH1750);
    std::cout << "Timer already running" << endl;
}

void BH1750::setworkmode(uint8_t workmode)
{
    BH1750::currentworkmode = workmode;
    std::cout << "set workmode success" << endl;
}

void BH1750::BH1750dataready()
{

    if (!BH1750Callback)
        return;
    std::cout << "start rec data" << endl;
    try
    {
        // uint8_t temp[3] = {0};
        // int fd =i2cOpen(1,0x23,0);
        uint8_t dest[2]=0;
        I2CreadBytes(BH1750_ADDR, BH1750_Register_ADDR, dest);
        std::cout << "rec data  success" << endl;
        real_lightvalue = lightcal(dest); // get real light inensity
         delete[] dest;
        BH1750Callback->hasSample(real_lightvalue);
    }
    catch (char const *msg)
    {
        std::cerr << "Error: " << msg << std::endl;
    }
}

float BH1750::lightcal(uint8_t *buf)
{
    float flight = 0;
    switch (BH1750::currentworkmode)
    {
    case continue_r1:
        flight = (buf[0] * 256 + buf[1]) * 1.0 / 1.2;
        std::cout << "currentR=r1" << endl;
        printf("%.2f", flight);
        break;
    case continue_r2:
        flight = (buf[0] * 256 + buf[1]) * 0.5 / 1.2;
        std::cout << "currentR=r2" << endl;
        printf("%.2f", flight);
        break;
    case continue_r3:
        flight = (buf[0] * 256 + buf[1]) * 4.0 / 1.2;
        std::cout << "currentR=r3" << endl;
        printf("%.2f", flight);
        break;
    default:
        std::cout << "WORK IN WRONG MODE.";
        break;
    }
    return flight;
}

void BH1750::gpioTimercallFuc(int gpio, int level, uint32_t tick, BH1750 *userdata)
{
        if(recflag)
		{
			userdata->BH1750dataready();
		}
		recflag = 1;

		int fd = i2cOpen(1, BH1750_ADDR, 0);
		if (fd < 0)
		{
         #ifdef DEBUG
	    fprintf(stderr, "Could not read");
         #endif
		}
		// I2CwriteByte(BH1750_ADDR,workmode);
		char config[] = {BH1750_Register_ADDR, BH1750::currentworkmode};
		i2cWriteDevice(fd, config, sizeof(config));
		std::cout << "write workmode success" << endl;
		i2cClose(fd);
}

void BH1750::stop()

{
    if (USThread)
    {

        USThread->join();
        delete USThread;
        USThread = NULL;
    }
}

void BH1750::BH1750WritePoweron()

{
    int fd = i2cOpen(1, BH1750_ADDR, 0);
    if (fd < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Could not read");
#endif
    }
    char cmd[] = {poweron};
    i2cWriteDevice(fd, cmd, sizeof(cmd));
    // I2CwriteByte(BH1750_ADDR,poweron); // subaddress 是寄存器地址，但BH1750寄存器地址可以省略，此外是否需要延时
    std::cout << "poweron success" << endl;
    i2cClose(fd);
}

void BH1750::BH1750WriteWorkMode(uint8_t workmode)
{
    int fd = i2cOpen(1, BH1750_ADDR, 0);
    if (fd < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Could not read");
#endif
    }
    // I2CwriteByte(BH1750_ADDR,workmode);
    char config[] = {BH1750_Register_ADDR, workmode};
    i2cWriteDevice(fd, config, sizeof(config));
    std::cout << "write workmode success" << endl;
    i2cClose(fd);
}

void BH1750::I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t *dest)
{
    int handle = i2cOpen(1, address, 0);
    if (handle < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Could not read");
#endif
        throw could_not_open_i2c;
    }

    int ret = i2cReadI2CBlockData(handle, subAddress, (char *)dest, 2);
    // i2cClose(handle);
    std::cout << "读到的值是=" << ret << endl;
    std::cout << dest[0] << std::endl;
    i2cClose(handle);
}
