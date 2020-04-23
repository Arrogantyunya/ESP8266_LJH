/************************************************************************************
 *
 * 代码与注释：刘家辉
 * 日期：2020/4/9
 *
 * 配置一些外设功能。
 *
 * 如有任何疑问，请发送邮件到： liujiahiu@qq.com
*************************************************************************************/

#include "fun_periph.h"

// Some_Peripherals Some_Peripheral;


// /*
//  @brief   : 设置继电器,SIM模块等外设引脚的模式
//  @param   : 无
//  @return  : 无
//  */
// void Some_Peripherals::Peripheral_GPIO_Pinmode(void)
// {
// 	pinMode(KCZJ1, OUTPUT);
// 	pinMode(KCZJ2, OUTPUT);
// 	pinMode(GPRS_PWRKEY_PIN, OUTPUT);
//   	pinMode(WLBG_PWR_PIN, OUTPUT);
// }


// /*
//  @brief   : 设置继电器，数字输入，模拟输入等外设引脚的状态
//  @param   : 无
//  @return  : 无
//  */
// void Some_Peripherals::Peripheral_GPIO_Config(void)
// {
// 	WLBG_PWR_ON;
// 	// RELAY_OUT1_OFF;
// 	// RELAY_OUT2_OFF;
// 	Some_Peripheral.Set_Relay(0,off);
// 	Some_Peripheral.Set_Relay(1,off);
// 	GPRS_PWRKEY_HIGH;
// }

// /*
//  @brief   : 选择要显示的LED灯（颜色），和闪烁的频率
//  @param   : 1.哪一个LED
// 			2.频率值（100ms/次）
//  @return  : 无
//  */
// // void Some_Peripherals::LED_Display(LED which_led, unsigned char freq)
// // {
// // 	Which_LED = which_led;
// // 	LED_Freq = freq;
// // 	Timer4.setPeriod(100000); // in microseconds, 100ms
// // 	Timer4.attachCompare1Interrupt(LED_Interrupt);
// // 	Timer4.setCount(0);
// // }

// /*
//  @brief   : 暂停所有LED闪烁，同时暂停定时器4
//  @param   : 无
//  @return  : 无
// */
// void Some_Peripherals::Stop_LED(void)
// {
// 	//Timer4.pause();
// 	// GREEN1_OFF;
// 	// GREEN2_OFF;
// 	// RED1_OFF;
// 	// RED2_OFF;
// }

// /*
//  @brief   : 恢复定时器4计数，LED开始闪烁
//  @param   : 无
//  @return  : 无
//  */
// void Some_Peripherals::Start_LED(void)
// {
// 	//Timer4.resume();
// }

// /*
//  @brief   : 外设引脚初始化。
//  @param   : void
//  @return  : 初始化完成返回true，失败返回false
//  */
// // bool Some_Peripherals::Peripheral_GPIO_Config_Init(void)
// // {
	
// // }

// /*
//  @brief   : 设置继电器开关
//  @param   : 无
//  @return  : 无
//  */
// void Some_Peripherals::Set_Relay(unsigned char way,bool value)//设置继电器
// {
// 	switch (way)
// 	{
// 	case 0:digitalWrite(KCZJ1, value);value>0?Serial.println("Relay1 OFF"):Serial.println("Relay1 ON"); break;
// 	case 1:digitalWrite(KCZJ2, value);value>0?Serial.println("Relay2 OFF"):Serial.println("Relay2 ON"); break;
// 	// case 2:digitalWrite(KCZJ3, value); break;
// 	// case 3:digitalWrite(KCZJ4, value); break;
// 	// case 4:digitalWrite(KCZJ5, value); break;
// 	// case 5:digitalWrite(KCZJ6, value); break;
// 	// case 6:digitalWrite(KCZJ7, value); break;
// 	// case 7:digitalWrite(KCZJ8, value); break;
// 	// case 8:digitalWrite(KCZJ9, value); break;
// 	// case 9:digitalWrite(KCZJ10, value); break;
// 	// case 10:digitalWrite(KCZJ11, value); break;
// 	// case 11:digitalWrite(KCZJ12, value); break;
// 	default:/*强制停止*/	Serial.println("异常!!强制停止");		break;
// 	}
// }


