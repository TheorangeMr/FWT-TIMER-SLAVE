#include "time_data_deal.h"

extern UART_HandleTypeDef huart2;

//读取LORA版本号指令
uint8_t LORA_Version[3] = {0x00,0x00,0x01};

uint8_t LORA_Baud_rate[7] ={0x08,0x04,0x04,0x00,0x01,0xc2,0x00};

/*
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
*/



extern uint8_t MS5_sign;
extern uint8_t MS100_sign;
extern uint8_t ReceiveBuff[RECEIVEBUFF_SIZE];
uint8_t Passed_sign = 0;
uint8_t Sys_time = 0;
uint8_t *p;

/*
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
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
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
*/
uint8_t Com_Establish(void) {
	uint8_t timer_slover_com = 0;
    if (ReceiveBuff[0] == Com_establishing2) {
			   *p = Com_establishing3;
        HAL_UART_Transmit(&huart2, p, 1, 0xffff);
			timer_slover_com = 1;
    }
		else{
			timer_slover_com = 0;
		}
    ReceiveBuff[0] = 0;
		return timer_slover_com;
}


/*
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
*/

extern TIM_HandleTypeDef htim1;


uint8_t EXTI3_Query(void) {
    if (Passed_sign == 1) {
			ReceiveBuff[0] = 0;
			*p = Time_stop;
			__HAL_TIM_SetCounter(&htim1, 0);                 //锟斤拷锟斤拷锟斤拷锟斤拷锟�
			__HAL_TIM_ENABLE(&htim1);                        //锟津开讹拷时锟斤拷锟斤拷锟叫憋拷志
			HAL_UART_Transmit(&huart2, p, 1, 0xffff);
			return 1;
    }
		else{
			return 0;
		}
}


/*
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
*/
uint8_t Stop_Check(void) 
{
	uint8_t Stop_sign = 0;
	if (ReceiveBuff[0] == Send_stop) {
		Stop_sign = 1;
		ReceiveBuff[0] = 0;
	}
	return Stop_sign;
}


/*
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
*/
void System_Reset(void)
{
	__HAL_TIM_DISABLE(&htim1);																								//关定时器1和2
	__HAL_TIM_DISABLE(&htim2);																								//关定时器1和2
	__HAL_TIM_SET_COUNTER(&htim1,0x0); 																		 		//计数器清零
	__HAL_TIM_SET_COUNTER(&htim2,0x0); 																		 		//计数器清零
	ReceiveBuff[0] = 0;																												//清零串口接收变量
	Passed_sign = 0;																													//清零外部中断标志
	MS5_sign = 0;																															//清零5ms中断标志
	Sys_time = 0;																															//清零系统记录时间
	EXTI->IMR |= (EXTI_LINE_3);																								//开外部中断
}


/*
	*@API_name：void System_Reset()
	*@brief：初始化位无线串口模块
	*@data：void
	*@return：void
	*@author：罗成
	*@timer：2023.1.5
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

