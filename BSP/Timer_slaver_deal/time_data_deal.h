/*******************************************
	*	@file ：  time_data_deal.h
	* @author：  罗成
	* @data： 2023.01.05
	* @version：  v1.0
*******************************************/
#ifndef __TIME_DATA_DEAL_H__
#define __TIME_DATA_DEAL_H__

#include "main.h"
#include "tim.h"


//设定从机几号，只需修改Slave_define为对应号数即可
#define Slave_define  1

//以下为主从机通讯时的信号密钥
//3次握手信号
#define Com_establishing1    ((0xa<<4)|Slave_define)                      //通讯已建立标志
#define Com_establishing2    ((0xb<<4)|Slave_define)                      //通讯已建立标志
#define Com_establishing3    ((0xc<<4)|Slave_define)                      //通讯已建立标志
//停止计数信号
#define Send_stop            ((0xf<<4)|Slave_define)                 //停止发送标志
#define Slaver_Time_stop     (Send_stop&0x0f)          //主机停止计时标志


uint8_t EXTI3_Query(void);
uint8_t Stop_Check(void);
uint8_t Com_Establish(void);
uint8_t Timer1_SignPlus(void);
void System_Reset(void);
void LORA_Init(void);

#endif


