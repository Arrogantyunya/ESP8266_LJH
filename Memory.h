#ifndef _Memory_H
#define _Memory_H

#include <EEPROM.h>
#include <ESP8266WiFi.h>

#define EEPROM_Capacity         100   	//EEPROM的容量
#define HARD_VERSION_BASE_ADDR  00      //硬件版本号起始地址（2字节）
#define SOFT_VERSION_BASE_ADDR  02      //软件版本号起始地址（2字节）
#define Device_ID_BASE_ADDR     04      //设备ID的起始地址（4字节）

union DEVEICE_ID
{
    unsigned int ID;
    byte id[4];
};

extern DEVEICE_ID Device_ID;


class EEPROM_Operations
{
private:
    /* data */
public:
    void EEPROM_Init(void);//初始化EP
    bool EEPROM_Reset(void);//将EP的值重置
};

class DEVEICE_INFORMATION : public EEPROM_Operations
{
private:
    /* data */
public:
    bool Save_DeviceID();   //保存设备ID
    void Read_DeviceID();   //读取设备ID
    bool Clean_DeviceID();  //清除设备ID
};


class Soft_Hard_Vertion : public EEPROM_Operations 
{
private:
    /* data */
public:
	void Save_hardware_version(unsigned char number_high, unsigned char number_low);
	void Save_Software_version(unsigned char number_high, unsigned char number_low);
	unsigned char Read_hardware_version(unsigned char number_addr);
	unsigned char Read_Software_version(unsigned char number_addr);
};


/*Create EEPROM object*/
extern EEPROM_Operations EEPROM_Operation;
/*Create software and hardware object*/
extern Soft_Hard_Vertion Vertion;
/*Create Device Information object*/
extern DEVEICE_INFORMATION Device_Information;

#endif