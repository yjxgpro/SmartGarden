#include "BH1750.h"

using namespace std;

uint8_t BH1750::currentworkmode=0;
float BH1750::real_lightvalue=0;
bool BH1750::RecFlag = false;
uint8_t BH1750::BH1750_buf[2]={0,0};

BH1750::BH1750(BH1750DeviceSettings bh1750deviceSettings)
{
    device = bh1750deviceSettings;
#ifdef DEBUG
    fprintf(stderr, "I2c: bus=%02x, BH1750Addr=%02x\n", device.i2c_bus, device.BHAddress);
#endif
    BH1750initgpio();
    BH1750WriteByte(Poweron);
    // 200ms make poweron success
    time_sleep(0.2); 
    BH1750WriteByte(Reset);
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
            gpioSetMode(LEDgpio, PI_OUTPUT);   // In demo, BH1750Callback control LED 
            gpioSetPWMfrequency(LEDgpio, pwmfrequency); //set PWM frequency
            gpioSetPWMrange(LEDgpio, pwmRange);   // set PWM range
            std::cout << "gpio initialisation success" << endl;
        }
    }
}

void BH1750::start()
{
    if (USThread)
    {
        std::cout << "thread already running" << endl;
        return;
    }
        RecFlag = false;  //Not calculate light until receive correct data
        USThread = new std::thread(&BH1750::run, this);   
}

void BH1750::run(BH1750 *BH1750) 
{
     std::cout << "create thread success" << endl;
     BH1750->BH1750WriteWorkMode(currentworkmode);
    gpioSetTimerFuncEx(Timer0, Reci2cDataPeriod, reinterpret_cast<gpioTimerFuncEx_t>(BH1750::gpioTimercallFuc), BH1750);
    if(RecFlag)
    {
        RecFlag = false;
        real_lightvalue = BH1750->lightcal(BH1750_buf); // 获取真实的光强值
        BH1750_buf[0]=0;
        BH1750_buf[1]=0;
        BH1750->BH1750dataready();
        } 
    else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 等待 1 秒钟再重试
        }
}

void BH1750::setworkmode(uint8_t workmode)
{
    BH1750::currentworkmode = workmode;
    std::cout << "set workmode success" << endl;
}

void BH1750::BH1750dataready()
{

    if (!BH1750Callback)
        std::cout<<"no pointer to bh1750callback"<<std::endl;
    else
    {
        BH1750Callback->hasSample(real_lightvalue);
    }
}

float BH1750::lightcal(uint8_t *buf)
{
    float flight = 0;
    switch (BH1750::currentworkmode)
    {
    case  Continue_H_M1:
        flight = (buf[0] * 256 + buf[1]) * 1.0 / 1.2;
        std::cout << "current Resolution = 1lx" << endl;
        printf("%.2f", flight);
        break;
    case Continue_H_M2:
        flight = (buf[0] * 256 + buf[1]) * 0.5 / 1.2;
        std::cout << "current Resolution = 0.5lx" << endl;
        printf("%.2f", flight);
        break;
    case Continue_H_M3:
        flight = (buf[0] * 256 + buf[1]) * 4.0 / 1.2;
        std::cout << "current Resolution = 4lx" << endl;
        printf("%.2f", flight);
        break;
    default:
        std::cout << "WORK IN WRONG MODE.";
        break;
        flight=-1.0;
    }
    return flight;
}

void BH1750::gpioTimercallFuc(BH1750 *userdata)
{
        if(userdata)
		{
			userdata->I2CreadBytes(BH1750_ADDR,BH1750_Register_ADDR,*BH1750_buf);
		}
		else
        {
            std::cerr << "Error: NULL pointer to BH1750 instance." << std::endl;
        }
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

int BH1750::BH1750WriteByte(uint8_t command)
{
    int handle=i2cOpen(device.i2c_bus,device.BH1750Addr,0);
    if (handle < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Could not read");
#endif
    }
    char cmd[] = {command};
   int ret = i2cWriteDevice(handle, cmd, sizeof(cmd)); 
   if(!ret)
   {
    std::cout << "Write One byte Fail" << endl;
    return -1;
   }
   else{
      std::cout << "Write One byte Success" << endl;
      return 0
   }
    
}

int BH1750::BH1750WriteWorkMode(uint8_t workmode)
{
    int handle = i2cOpen(BH1750_DEFAULT_I2C_BUS,BH1750_ADDR,0);
    if (handle < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Could not read");
#endif
       return -1
    }
    else
    {
    char config[] = {BH1750_Register_ADDR, workmode};
    i2cWriteDevice(handle, config, sizeof(config));
    std::cout << "write workmode success" << endl;
    return 0;
    }
    i2cClose(handle);
    }

int BH1750::I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t *dest)
{
    int handle = i2cOpen(BH1750_DEFAULT_I2C_BUS, address, 0);
    if (handle < 0)
    {
#ifdef DEBUG
        fprintf(stderr, "Could not read");
#endif
        return -1;
    }

    int ret = i2cReadI2CBlockData(handle, subAddress, (char *)dest, 2);
    if(ret!=2)
    {
        return -2;
    }
    else{
    RecFlag = true;
    return 0;
    }
    i2cClose(handle);
}
