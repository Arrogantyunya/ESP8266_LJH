// 
/*
    Name:     	ESP8266_LJH.ino
    Created:	2020/4/7 9:45:47
    Author:   	刘家辉
*/

#include <ESP8266WiFi.h>
#include "Memory.h"

/* 测试宏 */
#define DEBUG_PRINT 	false 	//打印调试信息
#define Reset_RTC 		false   //重置rtc时间
#define Get_RTC			false	//获取rtc时间
/* 使能宏 */
#define SOFT_HARD_VERSION	true 	//使能写入软件和硬件版本
#define USE_RTC				true 	//使用RTC
#define USE_TIMER			false 	//使用定时器
#define Device_information	true	//得到设备的信息
#define DeviceID			true	//使能写入设备ID
#define EEPROM_RESET		false 	//重置EEPROM的所有值【测试使用】
/* 替换宏 */
#define Software_version_high 	0x01  	//软件版本的高位
#define Software_version_low 	0x00	//软件版本的低位
#define Hardware_version_high 	0x01 	//硬件版本的高位
#define Hardware_version_low 	0x00  	//硬件版本的低位
#define interval                10000 	//间隔时间


#define PIN_LED1 D3
#define PIN_LED2 D6

#define DEFAULT_STASSID   "SHHOO"     //这里改成你的wifi名字
#define DEFAULT_STAPSW    "shhoo2018" //这里改成你的wifi密码

struct config_type
{
  char stassid[32];
  char stapsw[64];
};
config_type config;


ADC_MODE(ADC_VCC);//设置ADC模式为读取系统电压


//全局变量
bool Renewal_Time = false;//更新时间的标志位
unsigned long Old_Time = 0;//上一次记录的时间
String comdata = "";


//函数定义
void Serial_Port_Instruction();//串口指令配置

void setup() 
{
	delay(2000);
  	ESP.wdtEnable(5000);    //使能软件看门狗的触发间隔
	EEPROM_Operation.EEPROM_Init();//初始化EP

	pinMode(D1, OUTPUT);    // 初始化D1引脚为输出引脚
	pinMode(PIN_LED2,OUTPUT);
	analogWrite(PIN_LED2,0);

	attachInterrupt(digitalPinToInterrupt(D2), InterruptFunc, RISING);//设置中断号、响应函数、触发方式
	
	Serial.begin(9600);
	Serial.println();
	unsigned int br = Serial.baudRate();
	Serial.printf("Serial is %d bps \r\n", br);
	
	Serial.println(String("ESP8266当前系统电压(mV):") + ESP.getVcc());

#if	EEPROM_RESET	//重置EEPROM的所有值【测试使用】
	EEPROM_Operation.EEPROM_Reset();//重置EEPROM的所有值【测试使用】
#endif

#if Reset_RTC	//重置rtc时间

#endif

#if Device_information	//设备信息查看
	FlashMode_t ideMode = ESP.getFlashChipMode();
	String coreVersion = ESP.getCoreVersion();
	Serial.print(F("Arduino Core For ESP8266 Version: "));
	Serial.println(coreVersion);
	Serial.printf("Flash real id（唯一标识符）:   %08X\n", ESP.getFlashChipId());
	Serial.printf("Flash 实际大小: %u KBytes\n", ESP.getFlashChipRealSize()/1024);
	Serial.printf("IDE配置Flash大小: %u KBytes,往往小于实际大小\n", ESP.getFlashChipSize()/1024);
	Serial.printf("IDE配置Flash频率 : %u MHz\n", ESP.getFlashChipSpeed()/1000000);
	Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
#endif

#if SOFT_HARD_VERSION	//保存软硬件版本至EEPROM
	Serial.println("");

	//软件版本存储程序
	if (Software_version_high == Vertion.Read_Software_version(SOFT_VERSION_BASE_ADDR) &&
		Software_version_low == Vertion.Read_Software_version(SOFT_VERSION_BASE_ADDR + 1))
	{
		Serial.println(String("Software_version is V") + String(Software_version_high, HEX) + "." + String(Software_version_low, HEX));
	}
	else
	{
		Vertion.Save_Software_version(Software_version_high, Software_version_low);
		Serial.println(String("Successfully store the software version, the current software version is V") + String(Software_version_high, HEX) + "." + String(Software_version_low, HEX));
	}
	//硬件版本存储程序
	if (Hardware_version_high == Vertion.Read_hardware_version(HARD_VERSION_BASE_ADDR) &&
		Hardware_version_low == Vertion.Read_hardware_version(HARD_VERSION_BASE_ADDR + 1))
	{
		Serial.println(String("Hardware_version is V") + Hardware_version_high + "." + Hardware_version_low);
	}
	else
	{
		Vertion.Save_hardware_version(Hardware_version_high, Hardware_version_low);
		Serial.println(String("Successfully store the hardware version, the current hardware version is V") + Hardware_version_high + "." + Hardware_version_low);
	}
#endif

#if DeviceID	//保存设备ID信息至EEPROM
	Device_Information.Save_DeviceID();//保存设备ID
#endif
	
	bool Connect_WiFi = false;//是否连接上WIFI

	WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);//启动STA模式，并连接到wifi网络

	Serial.print(String("Connecting to ") + DEFAULT_STASSID);

	unsigned long Time = millis();//记录连接的时间

	//判断网络状态是否连接上，没连接上就延时500ms，并且打出一个点，模拟连接过程
	while ((WiFi.status() != WL_CONNECTED) && millis() - Time <= interval)
	{
		ESP.wdtFeed();//喂狗
		delay(500);Serial.print(". ");
	}
	Serial.println("");

	if (WiFi.status() == WL_CONNECTED)
	{
		Connect_WiFi = true;
		Serial.print(String("Successfu Lconnection, IP address: "));
		//输出station IP地址，这里的IP地址由DHCP分配
		Serial.println(WiFi.localIP());
	}
	else
	{
		Connect_WiFi = false;
		Serial.println("Fail !!! Connection Timeout");
	}

	if(Connect_WiFi)
	{
		Serial.println("All initialization completed,Welcome to use ~(*^__^*)~");
	}
	else
	{
		//初始化异常
	}
}
 
void loop() 
{
  ESP.wdtFeed();//喂狗

  Serial_Port_Instruction();//串口指令配置

  if (!Renewal_Time)
  {
    Old_Time = millis();
    Renewal_Time = true;
    // Serial.println(String("Old_Time = ") + Old_Time);
  }
  
  if (millis() - Old_Time >= interval)
  {
    // Serial.println(String("Time = ") + millis());
    
    Renewal_Time = false;
  }
}

/**
 * 中断响应函数
 */ 
ICACHE_RAM_ATTR void InterruptFunc()
{
  Serial.println("Hello ESP8266");
}

/*  
 *
 */
void Serial_Port_Instruction()//串口指令配置
{
	unsigned long Time = millis();
  	while ((Serial.available() > 0) && (millis() - Time <= 1500))
	{
		comdata += char(Serial.read());  //每次读一个char字符，并相加
		delay(2);
	}

	if (comdata.length() > 0)
	{
		comdata.toUpperCase();
		Serial.println(comdata);
		if (comdata.startsWith("SET:"))
		{
			
			comdata = "";
		}
		else if (comdata == String("EEPROM_RESET"))
		{
			Serial.println("开始擦除EEPROM...");
			
			EEPROM_Operation.EEPROM_Reset();

			comdata = "";
		}
		else if (comdata == String("XXXX"))
		{
			Serial.println("XXXX");
			comdata = "";
		}
		
		else
		{
			comdata = "";
			Serial.println("输入错误 <Com_Set_Cyclic_interval>");
		}
	}
}