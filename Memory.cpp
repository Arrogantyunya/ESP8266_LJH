/************************************************************************************
 *
 * 代码与注释：刘家辉
 * 日期：2020/4/8
 * 该文件主要功能是
 * 
 * 
 *
 * 如有任何疑问，请发送邮件到： liujiahiu@qq.com
*************************************************************************************/
#include "Memory.h"

/*创建EEPROM操作对象*/
EEPROM_Operations EEPROM_Operation;
/*创建软件硬件版本对象*/
Soft_Hard_Vertion Vertion;
/*创建设备信息操作对象*/
DEVEICE_INFORMATION Device_Information;


DEVEICE_ID Device_ID;


/*
 @brief     : 初始化EP
 @para      : 无
 @return    : 无
 */
void EEPROM_Operations::EEPROM_Init(void)
{
    EEPROM.begin(EEPROM_Capacity);//EEPROM申请100字节的空间
}

/*
 @brief     : 重置EEPROM的值
 @para      : 无
 @return    : true or false
 */
bool EEPROM_Operations::EEPROM_Reset(void)
{
	for (size_t i = 0; i < EEPROM_Capacity; i++)
    {
        EEPROM.write(i,0x00);
    }
    EEPROM.end(); //保存更改的数据

    EEPROM.begin(EEPROM_Capacity);//重新申请内存

    //检查EEPROM是否真的擦除成功
    unsigned long data = 0;
    for (size_t i = 0; i < EEPROM_Capacity; i++)
    {
        data += EEPROM.read(i);
        if (data >= 65535)
        {
            data = 1;
        }
    }

    if (data == 0)
    {
        Serial.println("擦除EEPROM完毕");
        return true;
    }
    else
    {
        Serial.println("擦除EEPROM失败");
        return false;
    }
}

//-------------------------------------------------------

/*
 @brief     : 保存设备ID
 @para      : 
 @return    : None
 */
bool DEVEICE_INFORMATION::Save_DeviceID()   //保存设备ID
{
    Device_ID.ID = ESP.getFlashChipId();
    
    for (size_t i = 0; i < sizeof(Device_ID); i++)
    {
        EEPROM.write(Device_ID_BASE_ADDR+i,Device_ID.id[i]);
    }

    EEPROM.end(); //保存更改的数据

    EEPROM.begin(EEPROM_Capacity);//重新申请内存

    if (EEPROM.read(Device_ID_BASE_ADDR) == Device_ID.id[0] && 
        EEPROM.read(Device_ID_BASE_ADDR+1) == Device_ID.id[1] &&
        EEPROM.read(Device_ID_BASE_ADDR+2) == Device_ID.id[2] &&
        EEPROM.read(Device_ID_BASE_ADDR+3) == Device_ID.id[3])
    {
        Serial.println(String("保存设备ID成功...") + "设备ID：" + String(Device_ID.ID,HEX));
        return true;
    }
    else
    {
        Serial.println("保存设备ID失败");
        return false;
    }
}

/*
 @brief     : 读取设备ID
 @para      : 
 @return    : None
 */
unsigned int DEVEICE_INFORMATION::Read_DeviceID()   //读取设备ID
{
    for (size_t i = 0; i < sizeof(Device_ID); i++)
    {
        Device_ID.id[i] = EEPROM.read(Device_ID_BASE_ADDR+i);
    }
    Serial.println(String("Device_ID.ID = ") + Device_ID.ID);
    return Device_ID.ID;
}

/*
 @brief     : 清除设备ID
 @para      : 
 @return    : None
 */
bool DEVEICE_INFORMATION::Clean_DeviceID()   //清除设备ID
{
    for (size_t i = 0; i < sizeof(Device_ID); i++)
    {
        EEPROM.write(Device_ID_BASE_ADDR+i,0x00);
    }

    EEPROM.end(); //保存更改的数据

    EEPROM.begin(EEPROM_Capacity);//重新申请内存

    if (EEPROM.read(Device_ID_BASE_ADDR) == 0x00 && 
        EEPROM.read(Device_ID_BASE_ADDR+1) == 0x00 &&
        EEPROM.read(Device_ID_BASE_ADDR+2) == 0x00 &&
        EEPROM.read(Device_ID_BASE_ADDR+3) == 0x00)
    {
        Serial.println(String("清除设备ID成功..."));
        return true;
    }
    else
    {
        Serial.println("清除设备ID失败");
        return false;
    }
}



//-------------------------------------------------------
/*
 @brief     : 保存该设备的硬件版本号
			  Save device's hardware version.
 @para      : Version number high byte, version number low byte.
 @return    : None
 */
void Soft_Hard_Vertion::Save_hardware_version(unsigned char number_high, unsigned char number_low)
{
	EEPROM.write(HARD_VERSION_BASE_ADDR, number_high);
	EEPROM.write(HARD_VERSION_BASE_ADDR + 1, number_low);

    EEPROM.end(); //保存更改的数据

    EEPROM.begin(EEPROM_Capacity);//重新申请内存
}

/*
 @brief     : 保存该设备的软件版本号
			  Save device's software version.
 @para      : Version number high byte, version number low byte.
 @return    : None
 */
void Soft_Hard_Vertion::Save_Software_version(unsigned char number_high, unsigned char number_low)
{
	EEPROM.write(SOFT_VERSION_BASE_ADDR, number_high);
	EEPROM.write(SOFT_VERSION_BASE_ADDR + 1, number_low);

    EEPROM.end(); //保存更改的数据

    EEPROM.begin(EEPROM_Capacity);//重新申请内存
}

unsigned char Soft_Hard_Vertion::Read_hardware_version(unsigned char number_addr)
{
	unsigned char version = EEPROM.read(number_addr);
	return version;
}

unsigned char Soft_Hard_Vertion::Read_Software_version(unsigned char number_addr)
{
	unsigned char version = EEPROM.read(number_addr);
	return version;
}