#include "BH1750.h"
#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <iostream>
#include "pigpio.h"

using namespace std

#ifndef NDEBUG
#define DEBUG
#endif

BH1750::BH1750(BH1750DeviceSettings bh1750deviceSettings) {
	device = bh1750deviceSettings;
#ifdef DEBUG
	fprintf(stderr,"I2c: bus=%02x, BH1750Addr=%02x\n",
		device.i2c_bus,device.BHAddress);
#endif
}

void BH1750::start()
{
    if (USThread)
    {
        std::cout << "thread already running";
        //throw "ERROR: Thread already running.";
    }
        USThread = new std::thread(run, this);//run作线程入口，this作传入参数
}

void BH1750::run()
{
// todo 
// 发送地址寻址
  uint8_t word;  //2 bytes
  float real_light;
  BH1750WritePoweron(address, device.BHAddress,poweron);
  BH1750WriteWorkMode(address, device.BHAddress,continue_r1);
  word = BH1750RecData(address, device.BHAddress,0,1);
  real_light=BH1750::lightcal(&BH1750_buf);
// 读取参-数
// 根据分辨率计算光照强度
}

void BH1750::BH1750initgpio()
{
	
}
void BH1750::stop()
{
    running = 0;
    if (USThread)
    {
        USThread->join();
        delete USThread;
        USThread = NULL;
    }
}

void BH1750::BH1750WritePoweron(uint8_t address, uint8_t subAddress, uint8_t data)
{
    return I2CwriteByte(address, device.BHAddress,poweron) // 不知道address是啥
}

void BH1750WriteWorkMode(uint8_t address, uint8_t subAddress, uint8_t data)
{
	return I2CwriteByte(address, device.BHAddress,continue_r1)
}

uint8_t BH1750::BH1750RecData(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
    return I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count)   //count 是2吗？两个字节
}

uint8_t BH1750::BH1750DataReady()
{
    BH1750_buf[0] = word&0x0f;  //读取的
    BH1750_buf[1] = word&0xf0;
}

float BH1750::lightcalc(float*buf)
{
	float flight;
    switch (device.currentmode)
    {
    case continue_r1:
        flight=(buf[0]*256+buf[1])*1.0/1.2;
        printf("%.2f",flight);
		return flight;
        break;
    case continue_r2:
        flight=(buf[0]*256+buf[1])*0.5/1.2;
        printf("%.2f",flight);
		return flight;
        break;
    case continue_r3:
        flight=(buf[0]*256+buf[1])*4.0/1.2;
        printf("%.2f",flight);
		return flight;
        break;
    default:
	flight = 0;
	return flight;
        break;
		if(flight==0)
		{
		#ifdef DEBUG
		throw "wrong workmode";
        #endif
		}
    }
}
 
// i2c read and write protocols
void BH1750::I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data)
{
	int fd = i2cOpen(device.i2c_bus, address, 0);
	if (fd < 0) {
#ifdef DEBUG
		fprintf(stderr,"Could not write %02x to %02x,%02x,%02x\n",data,device.i2c_bus,address,subAddress);
#endif
		throw could_not_open_i2c;
	}
	i2cWriteByteData(fd, subAddress, data);
	i2cClose(fd);
}

uint8_t BH1750::I2CreadByte(uint8_t address, uint8_t subAddress)
{
	int fd = i2cOpen(device.i2c_bus, address, 0);
	if (fd < 0) {
#ifdef DEBUG
		fprintf(stderr,"Could not read byte from %02x,%02x,%02x\n",device.i2c_bus,address,subAddress);
#endif
		throw could_not_open_i2c;
	}
	int data; // `data` will store the register data
	data = i2cReadByteData(fd, subAddress);
	if (data < 0) {
#ifdef DEBUG
		fprintf(stderr,"Could not read byte from %02x,%02x,%02x. ret=%d.\n",device.i2c_bus,address,subAddress,data);
#endif
		throw "Could not read from i2c.";
	}
	i2cClose(fd);
	return data;           
}

uint8_t BH1750::I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count)
{
	int fd = i2cOpen(device.i2c_bus, address, 0);
	if (fd < 0) {
#ifdef DEBUG
		fprintf(stderr,"Could not read %n byte(s) from %02x,%02x,%02x\n",count,device.i2c_bus,address,subAddress);
#endif
		throw could_not_open_i2c;
	}
	int ret = i2cReadWordData(fd,subAddress,(char*)dest,count);
	i2cClose(fd);
	if (ret != count) {
		throw "word read didn't work";
	}
	return ret;
}