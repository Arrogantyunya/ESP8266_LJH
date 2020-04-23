//
/*
    Name:     	ESP8266_LJH.ino
    Created:	2020/4/7 9:45:47
    Author:   	刘家辉
*/

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "Memory.h"

/* 测试宏 */
#define DEBUG_PRINT false //打印调试信息
#define Reset_RTC false	  //重置rtc时间
#define Get_RTC false	  //获取rtc时间
/* 使能宏 */
#define SOFT_HARD_VERSION 		true	//使能写入软件和硬件版本
#define USE_RTC 				true	//使用RTC
#define USE_Ticker				true	//使用定时器
#define USE_WDT					true	//使用看门狗
#define Device_information 		true 	//得到设备的信息
#define DeviceID_Save 			true	//使能写入设备ID
#define DeviceID_Clean 			false	//使能擦除设备ID
#define DeviceID_Read 			false	//使能读取设备ID
#define EEPROM_RESET 			false	//重置EEPROM的所有值【测试使用】
#define Connect_Fixed_Wifi		true	//连接固定的wifi
#define WEB_Configure_Network	false	//WEB配网
/* 替换宏 */
#define Software_version_high 0x01 //软件版本的高位
#define Software_version_low 0x00  //软件版本的低位
#define Hardware_version_high 0x01 //硬件版本的高位
#define Hardware_version_low 0x00  //硬件版本的低位
#define interval 10000			   //间隔时间

#define PIN_LED1 D3
#define PIN_LED2 D6

#define DEFAULT_STASSID "SHHOO"	   //这里改成你的wifi名字
#define DEFAULT_STAPSW "shhoo2018" //这里改成你的wifi密码

struct config_type
{
	char stassid[32];
	char stapsw[64];
};
config_type config;

ADC_MODE(ADC_VCC); //设置ADC模式为读取系统电压

//全局变量
bool Renewal_Time = false;	//更新时间的标志位
unsigned long Old_Time = 0; //上一次记录的时间
String comdata = "";

//函数定义
void Serial_Port_Instruction(void); //串口指令配置
bool Connect_fixed_WiFi(void);//连接Wifi
void Ticker_Callback();//定时回调函数

#if USE_Ticker
	Ticker Timer_Ticker;
	Ticker tickerSetHigh;
	Ticker tickerSetLow;
#endif

void setup()
{
	delay(1000);

	EEPROM_Operation.EEPROM_Init(); //初始化EP

	pinMode(LED_BUILTIN, OUTPUT);; // 初始化LED_BUILTIN引脚为输出引脚
	// pinMode(PIN_LED2, OUTPUT);
	// analogWrite(PIN_LED2, 0);

	// CHANGE（改变沿，电平从低到高或者从高到低）
	// RISING（上升沿，电平从低到高）
	// FALLING（下降沿，电平从高到低）
	attachInterrupt(digitalPinToInterrupt(D2), InterruptFunc, RISING); //设置中断号、响应函数、触发方式
	// detachInterrupt(D2);//该功能用于禁用指定GPIO引脚上的中断。

	Serial.begin(9600);
	Serial.println();
	unsigned int br = Serial.baudRate();
	Serial.printf("Serial is %d bps \r\n", br);

#if USE_WDT
	ESP.wdtEnable(5000);			//使能软件看门狗的触发间隔
#endif

#if USE_Ticker
	// 每隔25ms调用一次 setPin(0)
  	// tickerSetLow.attach_ms(250, setPin, 0);

  	// 每隔26ms调用一次 setPin(1)
  	// tickerSetHigh.attach_ms(260, setPin, 1);

	Timer_Ticker.attach(60, Ticker_Callback);//每隔xx秒周期性执行
	// Timer_Ticker.attach_ms(300,Ticker_Callback);//每隔xx毫秒周期性执行
	// Timer_Ticker.once(0.3,Ticker_Callback);//xx秒后只执行一次
	// Timer_Ticker.once_ms(300,Ticker_Callback);//xx毫秒后只执行一次
	
	// Timer_Ticker.detach();//停止Ticker
	
	if(Timer_Ticker.active())//Ticker是否激活状态
	{
		Serial.println("定时器初始化完成");
	}
	else
	{
		Serial.println("Error!定时器初始化异常!!!");	
	}
#endif

	Serial.println(String("ESP8266当前系统电压(mV):") + ESP.getVcc());

#if EEPROM_RESET					 //重置EEPROM的所有值【测试使用】
	EEPROM_Operation.EEPROM_Reset(); //重置EEPROM的所有值【测试使用】
#endif

#if Reset_RTC //重置rtc时间

#endif

#if Device_information //设备信息查看
	Serial.println();
	Serial.println("设备信息：");
	FlashMode_t ideMode = ESP.getFlashChipMode();
	String coreVersion = ESP.getCoreVersion();
	Serial.print(F("Arduino Core For ESP8266 Version: "));
	Serial.println(coreVersion);
	Serial.printf("Flash real id（唯一标识符）:   %08X\n", ESP.getFlashChipId());
	Serial.printf("Flash 实际大小: %u KBytes\n", ESP.getFlashChipRealSize() / 1024);
	Serial.printf("IDE配置Flash大小: %u KBytes,往往小于实际大小\n", ESP.getFlashChipSize() / 1024);
	Serial.printf("IDE配置Flash频率 : %u MHz\n", ESP.getFlashChipSpeed() / 1000000);
	Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
#endif

#if SOFT_HARD_VERSION //保存软硬件版本至EEPROM
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

#if DeviceID_Save						//保存设备ID信息至EEPROM
	Serial.println("");
	Device_Information.Save_DeviceID(); //保存设备ID
#endif

#if DeviceID_Clean						 //清除保存在EEPROM中的设备ID信息
	Device_Information.Clean_DeviceID(); //清除设备ID
#endif

#if DeviceID_Read //读取保存在EEPROM中的设备ID信息
	unsigned int Device_ID = Device_Information.Read_DeviceID();
	Serial.println(String("Device_ID = ") + Device_ID);
#endif

#if Connect_Fixed_Wifi	//连接固定的WIFI
	if (Connect_fixed_WiFi())
	{
		Serial.println("All initialization completed,Welcome to use ~(*^__^*)~");
	}
	else
	{
		//初始化异常
	}
#endif

#if WEB_Configure_Network	//WEB配置网络
	if()
	{
		Serial.println("");
	}
	else
	{
		//异常处理
		Serial.println("");
	}
#endif
}

void loop()
{
	ESP.wdtFeed(); //喂狗

	Serial_Port_Instruction(); //串口指令配置

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

/*
 @brief     : 中断响应函数
 @para      : 
 @return    : None
 */
ICACHE_RAM_ATTR void InterruptFunc(void)
{
	Serial.println(" <InterruptFunc>");
}

/*
 @brief     : 连接固定的WIFI
 @para      : 
 @return    : None
 */
bool Connect_fixed_WiFi(void)
{
	bool Connect_WiFi = false; //是否连接上WIFI

	WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW); //启动STA模式，并连接到wifi网络

	Serial.println("");
	Serial.print(String("Connecting to ") + DEFAULT_STASSID);

	unsigned long Time = millis(); //记录连接的时间

	//判断网络状态是否连接上，没连接上就延时500ms，并且打出一个点，模拟连接过程
	while ((WiFi.status() != WL_CONNECTED) && millis() - Time <= interval)
	{
		ESP.wdtFeed(); //喂狗
		delay(500);
		Serial.print(". ");
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

	return Connect_WiFi;
}

/*
 @brief     : 串口指令配置
 @para      : 
 @return    : None
 */
void Serial_Port_Instruction(void) //串口指令配置
{
	unsigned long Time = millis();
	while ((Serial.available() > 0) && (millis() - Time <= 1500))
	{
		comdata += char(Serial.read()); //每次读一个char字符，并相加
		delay(2);
	}

	if (comdata.length() > 0)
	{
		comdata.toUpperCase();
		Serial.println(comdata);
		if (comdata.startsWith("SET:"))
		{

		}
		else if (comdata == String("EEPROM_RESET"))
		{
			Serial.println("开始擦除EEPROM...");

			EEPROM_Operation.EEPROM_Reset();
		}
		else if (comdata == String("XXXX"))
		{
			Serial.println("XXXX");
		}
		else
		{
			Serial.println("输入错误 <Com_Set_Cyclic_interval>");
		}
		comdata = "";
	}
}

void Ticker_Callback()//定时回调函数
{
	Serial.println(" <Ticker_Callback>");
}

void setPin(int state) {
  digitalWrite(LED_BUILTIN, state);
}