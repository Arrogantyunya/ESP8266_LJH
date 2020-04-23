#ifndef _FUN_PERIPH_H
#define _FUN_PERIPH_H

#include <Arduino.h>

// #define on						0x00	//
// #define off						0x01	//

// //Selset Dev Board Type
// #define UNO			false
// #define MEGA2560	false
// #define STM32		true

// // Use Hardware Serial on Mega, Leonardo, Micro
// #if MEGA2560
// #define SerialAT Serial3
// #define GPRS_PWRKEY_PIN 4
// #elif UNO
// #define U_RX 2
// #define U_TX 3
// #define GPRS_PWRKEY_PIN 4
// #include "SoftwareSerial.h"
// SoftwareSerial SerialAT(U_RX, U_TX);
// #elif 	STM32
// #define SerialAT          Serial2
// #define GPRS_PWRKEY_PIN   PA15
// #define WLBG_PWR_PIN      PA5
// #define WLBG_PWR_ON       digitalWrite(WLBG_PWR_PIN, HIGH)
// #define WLBG_PWR_OFF      digitalWrite(WLBG_PWR_PIN, LOW)
// #define KCZJ1    PA3
// #define KCZJ2    PA4
// #define RELAY_OUT1_ON     digitalWrite(KCZJ1, LOW)
// #define RELAY_OUT1_OFF    digitalWrite(KCZJ1, HIGH)
// #define RELAY_OUT2_ON     digitalWrite(KCZJ2, LOW)
// #define RELAY_OUT2_OFF    digitalWrite(KCZJ2, HIGH)
// #else
// #define U_RX 2
// #define U_TX 3
// #define GPRS_PWRKEY_PIN 4
// #include "SoftwareSerial.h"
// SoftwareSerial SerialAT(U_RX, U_TX);
// #endif

// //根据硬件上的连接来更改
// #define GPRS_PWRKEY_HIGH    digitalWrite(GPRS_PWRKEY_PIN, HIGH)
// #define GPRS_PWRKEY_LOW     digitalWrite(GPRS_PWRKEY_PIN, LOW)

// #define MAX_OUT_NUM       2

// class Some_Peripherals {
// public:
// 	/*Configurate some functional pins*/
// 	void Peripheral_GPIO_Pinmode(void);
// 	/*Configurate some functional pins*/
// 	void Peripheral_GPIO_Config(void);
// 	// void LED_Display(LED which_led, unsigned char freq);
// 	void Stop_LED(void);
// 	void Start_LED(void);
// 	// bool Peripheral_GPIO_Config_Init(void);//外设引脚初始化。
// 	void Set_Relay(unsigned char way,bool value);//设置继电器
// }; 

// /*Create peripheral object*/
// extern Some_Peripherals Some_Peripheral;

#endif