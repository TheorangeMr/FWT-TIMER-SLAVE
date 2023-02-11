#include "time_data_deal.h"

extern UART_HandleTypeDef huart2;

//��ȡLORA�汾��ָ��
uint8_t LORA_Version[3] = {0x00,0x00,0x01};

uint8_t LORA_Baud_rate[7] ={0x08,0x04,0x04,0x00,0x01,0xc2,0x00};

/*
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
*/



extern uint8_t MS5_sign;
extern uint8_t MS100_sign;
extern uint8_t ReceiveBuff[RECEIVEBUFF_SIZE];
uint8_t Passed_sign = 0;
uint8_t Sys_time = 0;
uint8_t *p;

/*
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
*/

//����Ϊ50ms
uint8_t Timer1_SignPlus(void) {
    if (MS5_sign == 10) {
        Sys_time++;
        MS5_sign = 0;                   
    }
    return Sys_time;
}


/*
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
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
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
*/

extern TIM_HandleTypeDef htim1;


uint8_t EXTI3_Query(void) {
    if (Passed_sign == 1) {
			ReceiveBuff[0] = 0;
			*p = Time_stop;
			__HAL_TIM_SetCounter(&htim1, 0);                 //���������
			__HAL_TIM_ENABLE(&htim1);                        //�򿪶�ʱ�����б�־
			HAL_UART_Transmit(&huart2, p, 1, 0xffff);
			return 1;
    }
		else{
			return 0;
		}
}


/*
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
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
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
*/
void System_Reset(void)
{
	__HAL_TIM_DISABLE(&htim1);																								//�ض�ʱ��1��2
	__HAL_TIM_DISABLE(&htim2);																								//�ض�ʱ��1��2
	__HAL_TIM_SET_COUNTER(&htim1,0x0); 																		 		//����������
	__HAL_TIM_SET_COUNTER(&htim2,0x0); 																		 		//����������
	ReceiveBuff[0] = 0;																												//���㴮�ڽ��ձ���
	Passed_sign = 0;																													//�����ⲿ�жϱ�־
	MS5_sign = 0;																															//����5ms�жϱ�־
	Sys_time = 0;																															//����ϵͳ��¼ʱ��
	EXTI->IMR |= (EXTI_LINE_3);																								//���ⲿ�ж�
}


/*
	*@API_name��void System_Reset()
	*@brief����ʼ��λ���ߴ���ģ��
	*@data��void
	*@return��void
	*@author���޳�
	*@timer��2023.1.5
*/


void LORA_Init(void)
{
	uint8_t i = 0;
	//A39-T400A30D1a ��������ģʽ
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_RESET);
	//���ò�����Ϊ115200
	for(i = 0;i<7;i++)
	{
		HAL_UART_Transmit_DMA(&huart2, &LORA_Baud_rate[i], sizeof(LORA_Baud_rate));
	}
	//A39-T400A30D1a �˳�����ģʽ������һ�㹤��ģʽ
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
//	//AS32-TTL-100-C ��������ģʽ
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET);
//	//AS32-TTL-100-C �˳�����ģʽ������һ�㹤��ģʽ
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_RESET);	
}
