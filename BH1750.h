/******************************************************************************
2023, cheng cai, 2740656c@student.gla.ac.uk

This file prototypes the BH1750 class, implemented in BH1750.cpp. 
BH1750 light sensor is a part function of Project "SmartGarden",
please click https://github.com/yjxgpro/SmartGarden.git see more.
BH1750 datasheet is post on Github wiki "build manual" -> "instruction" page

Development environment specifics:
    Hardware Platform: Raspberry PI
    BH1750
******************************************************************************/
#ifndef NDEBUG
#define DEBUG
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <thread>
#include <pigpio.h>
#include <iostream>

using namespace std;

static const char could_not_open_i2c[] = "Could not open I2C.\n";
#define BH1750_ADDR 0x23
#define BH1750_DEFAULT_I2C_BUS 1
#define BH1750_Register_ADDR 0
#define EnoughLight 500
#define Timer0 0
#define Reci2cDataPeriod 220
#define LEDgpio 18
#define pwmRange 255
#define pwmfrequency 50

/**
 * Command writen from master to slave
 * workmode explanation:
 * - continue : continue detect light data
 * - onetime :only detect one time
 * - H : High resolution   - L : Low resolution
 * - M : Mode1/2/3 Resolution 1/0.5/4 lx 
 **/
enum BH1750_workmode
{
	Poweroff = 0x00,     
	Poweron = 0x01,
	Reset = 0x07,       // Reset Data Register in BH1750, only work when it poweron
	Continue_H_M1 = 0x10, // resolution 11x
	Continue_H_M2 = 0x11, // resolution .51x
	Continue_L_M3 = 0x13, // resolution 41x
	Onetime_H_M1 = 0x10, // resolution 11x
	Onetime_H_M2 = 0x11, // resolution .51x
	Onetime_L_M3 = 0x13, // resolution 41x
};

/**
 * Hardware related settings
 **/
struct BH1750DeviceSettings

{
	// I2C BH1750 address when BH1750 Addrline connect GND
	uint8_t BH1750Addr = BH1750_ADDR;
	// Default I2C bus number (most likely 1)
	uint8_t i2c_bus = BH1750_DEFAULT_I2C_BUS;
	// BH1750 data register address, BH1750 has only one register 
	uint8_t BH1750RegAddr = BH1750_DEFAULT_I2C_BUS;
};

/**
 * Callback interface where the callback needs to be
 * implemented by the host application.
 **/
class BH1750callback
{
public:
	// Called after a sample has arrived.
	virtual void hasSample(float bufdata) = 0;
};

/**
 * Main class for the BH1750 sensor which write <poweron>
 * <RegReset> command to BH1750, then create thread for 
 * Receiving BH1750 data. Create a timer(time is related to BH1750 work mode),
 * use TimerCallFuc Function to receive data, then pass the data to BH1750callback,
 * In "SmartGarden" Project, BH1750callback is a LED.
 **/
class BH1750
{

public:
     /** 
	 * BH1750 class constructor
	 * \param deviceSettings is defined in DeviceSettings
	 * The deviceSettings has default values for standard wiring.
	 * When object is created, initialise GPIO and write "Poweron","ResetReg" to BH1750
	 **/
	BH1750(BH1750DeviceSettings bh1750deviceSettings = BH1750DeviceSettings()); 
     
	//! \brief Starts thread for BH1750 , checks its not already running
	void start();
    
	/**
	//! \brief This is the interface function for thread startup.
	 * in run(), write "workmode" to BH1750,start a timer for receive SDA data
	 * when data is ready, calculate true light value and send it to 
	 * bh1750callback func.
	 **/
	static void run(BH1750 *BH1750);

	//! \brief Stops and tidies light thread.
	void stop();

	/**
	 * Sets the callback which receives the samples.
	 * \param cb Callback interface.
	 **/
	void setCallback(BH1750callback *cb)
	{ //  callback接口
		BH1750Callback = cb;
		std::cout << "set callback success" << endl;
	}

    //! \brief when object is killed, kill thread.
	~BH1750()
	{
		stop();
	}

   /**
    \brief user can setup BH1750 workmode in this Fuc
    \param userdata is command writen to BH1750 in "BH1750_workmode" list
   **/
	void setworkmode(uint8_t userdata); 

private:

	std::thread *USThread = NULL; 

	BH1750DeviceSettings device;

	// Callback interface which is registered with the main program.
	BH1750callback *BH1750Callback = nullptr;

	// a static float data used to store true light value in thread
	// initialised in BH1750.cpp
	static float real_lightvalue;

	// a static array used to store data read in timercallfuc
	// initialised in BH1750.cpp
	static uint8_t BH1750_buf[2]; 

    // a static bool Flag used to show receiving 2 bytes successfully in timercallfuc
	// initialised in BH1750.cpp
	static bool RecFlag;

	// a static uint8_t data used to show receiving 2 bytes successfully in timercallfuc
	// setup from setworkmode() in main programme
	static uint8_t currentworkmode;

	// pigpio initialise when BH1750 object is created
	void BH1750initgpio();
    
    /**
    \brief user can write one byte to regester in this Fuc
    \param userdata is command writen to BH1750 in "BH1750_workmode" list,
	decided by user
	output:
	      -1   write wrong data, not 1 byte
	       0   write right data, 1 byte
    **/
	int BH1750::BH1750WriteByte(uint8_t command)

    /**
    \brief user can setup BH1750 workmode in this Fuc
    \param userdata is command writen to BH1750 in "BH1750_workmode" list,
	decided by user
    **/
	int BH1750WriteWorkMode(uint8_t workmode);

	 /**
    \brief calculate true light value due to currentworkmode and data read in timercallfunc
    \param buf is array where stores light data read in timercallfuc
	// output :
	 a float light value (unit:lx)
	 when work in onetime workmode, output is -1.0
    **/
	float lightcal(uint8_t *buf);

	// After receive SDA 2 bytes, RecFlag turn ture, this callback function will be called 
	// in run() thread 
	void BH1750dataready();

	// timer callback handler registered with from pigpio
	// The userdata contains a pointer to this class instance.
	static void gpioTimercallFuc(BH1750 *userdata);

	// I2CreadBytes() -- Read 2 bytes of BH1750 regester
	// Input:
	//    - address = The 7-bit I2C address of the slave device.
	//    - subAddress = The register to begin reading.
	//    - * dest = Pointer to an array where we'll store the readings.
	// Output: 
	//      -2   i2c communication fail
	//      -1   receive wrong data, not 2 bytes
	//       0   receive 2 bytes successfully
	int I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t *dest);
};

