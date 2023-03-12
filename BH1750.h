#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <thread>
#include <pigpio.h>
#include <linux/i2c-dev.h>
#include <errno.h>

using namespace std;

static const char could_not_open_i2c[] = "Could not open I2C.\n";

// #define ISR_TIMEOUT 1000

#define BH1750_ADDR 0x23
#define BH1750_DEFAULT_I2C_BUS 1
#define BH1750_Register_ADDR 0
#define I2C_slkgpio 1
#define I2C_sdagpio 2
#define LED_control1 3
#define LED_control2 4
#define EnoughLight 1000

enum BH1750_workmode
{
	poweroff=0x00,
	poweron=0x01,
    reset=0x07,
	continue_r1=0x10,   //resolution 11x
	continue_r2=0x11,   //resolution .51x
	continue_r3=0x13,   //resolution 41x
};

// BH1750 hardware 
struct BH1750DeviceSettings
{
	// I2C BH1750 address
	uint8_t BHAddress = BH1750_ADDR;

    // Default I2C bus number (most likely 1)
	unsigned i2c_bus = BH1750_DEFAULT_I2C_BUS;

	// I2C_SLK_gpio
	uint8_t bh1750_i2c_slkgpio = I2C_slkgpio;

	// I2C_SDA_gpio
	uint8_t bh1750_i2c_sdagpio = I2C_sdagpio;
};

class BH1750
{
    public:
	BH1750(BH1750DeviceSettings bh1750deviceSettings = BH1750DeviceSettings());  //有初始值直接 = BH1750DeviceSettings(),用户也可以自己改？

	//! \brief Starts thread for light, checks its not already running
    void start();

	 //! \brief Runs light routine
    static void run(BH1750 *BH1750);

    //! \brief Stops + tidies light sensor thread.
    void stop();

	/**
	 * Sets the callback which receives the samples at the sampling rate.
	 * \param cb Callback interface.
	 **/
	void setCallback(BH1750callback* cb) {           //  callback接口
		BH1750Callback = cb;}

	~BH1750()
	 {
		stop();
	 }

	uint16_t setworkmode(uint16_t userdata)
	 {
		currentworkmode = userdata;
		return currentworkmode;
	 }

	private:
    std::thread* USThread = NULL;          // 线程 
	BH1750DeviceSettings device;
	uint16_t currentworkmode;
	// current BH1750workmode continute_resoulution 11x
	BH1750callback* BH1750Callback = nullptr;
	uint16_t BH1750twobytes=0;  //2 bytes
	float real_lightvalue=0;
	int8_t BH1750_buf[2]={0};                   // BH1750读取的数据, 之后可能传给QT?,先放这里
	bool LEDTri = false;                    // 根据计算结果决定是否打开开关
    
	void BH1750initgpio();
	void BH1750WritePoweron();
	void BH1750WriteWorkMode(uint8_t workmode);
	uint8_t BH1750RecData(uint8_t * dest, uint8_t count);
	float lightcal(float*buf);     //在设定分辨率下计算光照强度
	void BH1750dataready();

		// I2CwriteByte() -- Write a byte out of I2C to a register in the device
	// Input:
	//    - address = The 7-bit I2C address of the slave device.
	//    - subAddress = The register to be written to.
	//    - data = Byte to be written to the register.
	void I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data);
    
	// I2CreadByte() -- Read a single byte from a register over I2C.
	// Input:
	//    - address = The 7-bit I2C address of the slave device.
	//    - subAddress = The register to be read from.
	// Output:
	//    - The byte read from the requested address.
	uint8_t I2CreadByte(uint8_t address, uint8_t subAddress);
    
	// I2CreadBytes() -- Read a series of bytes, starting at a register via SPI
	// Input:
	//    - address = The 7-bit I2C address of the slave device.
	//    - subAddress = The register to begin reading.
	//     - * dest = Pointer to an array where we'll store the readings.
	//    - count = Number of registers to be read.
	// Output: No value is returned by the function, but the registers read are
	//         all stored in the *dest array given.
	uint8_t I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count);
};


class BH1750callback {
public:
        // Called after a sample has arrived.
        virtual void hasSample(float * bufdata) = 0;
};

