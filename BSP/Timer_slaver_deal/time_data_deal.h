#ifndef __TIME_DATA_DEAL_H__
#define __TIME_DATA_DEAL_H__

#include "main.h"
#include "tim.h"

#define Com_establishing1 0xa1         //通讯已建立标志
#define Com_establishing2 0xb1         //通讯已建立标志
#define Com_establishing3 0xc1         //通讯已建立标志
#define Send_stop 0X88                //停止发送标志
#define Time_stop 0X08                //主机停止计时标志

uint8_t EXTI3_Query(void);
uint8_t Stop_Check(void);
uint8_t Com_Establish(void);
uint8_t Timer1_SignPlus(void);
void System_Reset(void);
void LORA_Init(void);

#endif


