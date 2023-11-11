/*******************************************
	*	@file ：  time_data_deal.c
	* @author：  罗成
	* @data： 2023.01.05
	* @version：  v1.0
*******************************************/


#include "time_data_deal.h"

extern UART_HandleTypeDef huart2;

//读取LORA版本号指令
uint8_t LORA_Version[3] = {0x00,0x00,0x01};

uint8_t LORA_Baud_rate[7] ={0x08,0x04,0x04,0x00,0x01,0xc2,0x00};


extern __IO uint8_t MS5_sign;
extern __IO uint8_t MS100_sign;
__IO uint8_t ReceiveBuff = 0;
__IO uint8_t Passed_sign = 0;
uint8_t Sys_time = 0;

/*
	*@API_name：Timer1_SignPlus()
	*@brief：50ms计数
	*@papam：None
	*@retval：Sys_time
	*@author：罗成
	*@data：2023.1.5
*/

//周期为50ms
uint8_t Timer1_SignPlus(void) {
    if (MS5_sign == 10) {
        Sys_time++;
        MS5_sign = 0;                   
    }
    return Sys_time;
}


/*
	*@API_name：Com_Establish()
	*@brief：检测主机配对信号、检测到后返回主机配对信号
	*@papam：None
	*@retval：timer_slover_com
	*@author：罗成
	*@data：2023.1.5
*/
uint8_t Com_Establish(void) {
	uint8_t timer_slover_com = 0;
	uint8_t i = 10;
    if (ReceiveBuff == Com_establishing2){
			  timer_slover_com = Com_establishing3;
			  EXTI->IMR |= (0x08);                 //开启外部中断
			  while(i--)
				{
					HAL_UART_Transmit(&huart2,(uint8_t * ) &timer_slover_com , 1, 0xffff);
					HAL_Delay(50);
				}
    }
		else{
			timer_slover_com = 0;
		}
    ReceiveBuff = 0;
		return timer_slover_com;
}


/*
	*@API_name：EXTI3_Query()
  *@brief：向主机发送触发结果
	*@papam：None
	*@retval：。。。
	*@author：罗成
	*@data：2023.1.5
*/

extern TIM_HandleTypeDef htim1;


uint8_t EXTI3_Query(void) {
	uint8_t ch = 0;
    if (1 == Passed_sign) {
			ReceiveBuff = 0;
			ch = Slaver_Time_stop;
			__HAL_TIM_SetCounter(&htim1, 0);           
			__HAL_TIM_ENABLE(&htim1);                       
			HAL_UART_Transmit(&huart2, (uint8_t *) &ch , 1, 0xffff);
			return 1;
    }
		else{
			return 0;
		}
}


/*
	*@API_name：Stop_Check()
	*@brief：检测主机返回状态
	*@papam：None
	*@retval：Stop_sign
	*@author：罗成
	*@data：2023.1.5
*/
uint8_t Stop_Check(void) 
{
	uint8_t Stop_sign = 0;
	if (ReceiveBuff == Send_stop) {
		Stop_sign = 1;
		ReceiveBuff = 0;
	}
	return Stop_sign;
}


/*
	*@API_name：System_Reset()
	*@brief：重置系统于待计时状态
	*@papam：None
	*@retval：None
	*@author：罗成
	*@data：2023.1.5
*/
void System_Reset(void)
{
	__HAL_TIM_DISABLE(&htim1);																								//关定时器1
	__HAL_TIM_SET_COUNTER(&htim1,0x0); 																		 		//计数器清零
	ReceiveBuff = 0;																												//清零串口接收变量
	Passed_sign = 0;																													//清零外部中断标志
	MS5_sign = 0;																															//清零5ms中断标志
	Sys_time = 0;																															//清零系统记录时间
//	EXTI->IMR |= (EXTI_ LINE_3);																								//开外部中断
}


/*
	*@API_name：LORA_Init()
	*@brief：修改无线模块工作参数
	*@papam：None
	*@retval：None
	*@author：罗成
	*@data：2023.1.5
*/


void LORA_Init(void)
{
	uint8_t i = 0;
	//A39-T400A30D1a 进入配置模式
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_RESET);
	//配置波特率为115200
	for(i = 0;i<7;i++)
	{
		HAL_UART_Transmit_DMA(&huart2, &LORA_Baud_rate[i], sizeof(LORA_Baud_rate));
	}
	//A39-T400A30D1a 退出配置模式，进入一般工作模式
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
//	//AS32-TTL-100-C 进入配置模式
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET);
//	//AS32-TTL-100-C 退出配置模式，进入一般工作模式
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_RESET);	
}

