/*
 * ATP_Mode.c
 *
 *  Created on: Sep 21, 2024
 *      Author: VIMAL
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

#define POWER_CTRL_1 			1
#define POWER_CTRL_2 			2
#define INPUT_GPIO_1 			3
#define INPUT_GPIO_2 			4
#define INPUT_GPIO_3 			5
#define INPUT_GPIO_4 			6
#define INPUT_GPIO_5 			7
#define OUTPUT_GPIO_1 			8
#define OUTPUT_GPIO_2 			9
#define TS_1		 			10
#define TS_2		 			11
#define TS_3		 			12
#define TS_4		 			13
#define TS_5		 			14
#define TS_6		 			15
#define TS_7		 			16
#define TS_8		 			17
#define TS_9		 			18
#define TS_10		 			19
#define TS_11		 			20
#define TS_12		 			21
#define RTC_CHECK	 			22
#define NAND_FLASH_WRITE		23
#define NAND_FLASH_READ		 	24
#define NAND_FLASH_ERASE		25


extern SRAM_HandleTypeDef hsram1;
extern UART_HandleTypeDef huart5;


uint8_t *test_nbr_1 = "01";
uint8_t *test_nbr_2 = "02";
uint8_t *test_nbr_3 = "03";
uint8_t *test_nbr_4 = "04";
uint8_t *test_nbr_5 = "05";
uint8_t *test_nbr_6 = "06";
uint8_t *test_nbr_7 = "07";
uint8_t *test_nbr_8 = "08";
uint8_t *test_nbr_9 = "09";
uint8_t *test_nbr_10 = "10";
uint8_t *test_nbr_11 = "11";
uint8_t *test_nbr_12 = "12";
uint8_t *test_nbr_13 = "13";
uint8_t *test_nbr_14 = "14";
uint8_t *test_nbr_15 = "15";
uint8_t *test_nbr_16 = "16";
uint8_t *test_nbr_17 = "17";
uint8_t *test_nbr_18 = "18";
uint8_t *test_nbr_19 = "19";
uint8_t *test_nbr_20 = "20";
uint8_t *test_nbr_21 = "21";
uint8_t *test_nbr_22 = "22";
uint8_t *test_nbr_23 = "23";
uint8_t *test_nbr_24 = "24";
uint8_t *test_nbr_25 = "25";

extern float temp_sensor_1;
extern float temp_sensor_2;
extern float temp_sensor_3;
extern float temp_sensor_4;
extern float temp_sensor_5;
extern float temp_sensor_6;
extern float temp_sensor_7;
extern float temp_sensor_8;
extern float temp_sensor_9;
extern float temp_sensor_10;
extern float temp_sensor_11;
extern float temp_sensor_12;

uint8_t command_str[15]={0};
uint8_t command_str_index = 0;
uint8_t ATP_test = 0 ;
uint8_t ATP_test_number = 0 ;

uint8_t ATP_buffer[100];


/*
 * variables declaration for NAND Flash
 */
extern uint8_t NAND_Buf[15];
extern uint8_t NAND_index;
extern uint8_t NAND_flag;
//uint8_t ATP_test = 0 ;
//uint8_t ATP_test_number = 0 ;




void ATP_operation()
{
	HAL_GPIO_TogglePin(GPIOE, LED1_Pin);
	HAL_Delay(200);
	//ATP_test = 1;
	if(ATP_test)
	{
		ATP_test = 0 ;

		if (strstr(command_str, test_nbr_1))
		{
			ATP_test_number = 1;
		}
		else if (strstr(command_str, test_nbr_2))
		{
			ATP_test_number = 2;
		}
		else if (strstr(command_str, test_nbr_3))
		{
			ATP_test_number = 3;
		}
		else if (strstr(command_str, test_nbr_4))
		{
			ATP_test_number = 4;
		}
		else if (strstr(command_str, test_nbr_5))
		{
			ATP_test_number = 5;
		}
		else if (strstr(command_str, test_nbr_6))
		{
			ATP_test_number = 6;
		}
		else if (strstr(command_str, test_nbr_7))
		{
			ATP_test_number = 7;
		}
		else if (strstr(command_str, test_nbr_8))
		{
			ATP_test_number = 8;
		}
		else if (strstr(command_str, test_nbr_9))
		{
			ATP_test_number = 9;
		}
		else if (strstr(command_str, test_nbr_10))
		{
			ATP_test_number = 10;
		}
		else if (strstr(command_str, test_nbr_11))
		{
			ATP_test_number = 11;
		}
		else if (strstr(command_str, test_nbr_12))
		{
			ATP_test_number = 12;
		}
		else if (strstr(command_str, test_nbr_13))
		{
			ATP_test_number = 13;
		}
		else if (strstr(command_str, test_nbr_14))
		{
			ATP_test_number = 14;
		}
		else if (strstr(command_str, test_nbr_15))
		{
			ATP_test_number = 15;
		}
		else if (strstr(command_str, test_nbr_16))
		{
			ATP_test_number = 16;
		}
		else if (strstr(command_str, test_nbr_17))
		{
			ATP_test_number = 17;
		}
		else if (strstr(command_str, test_nbr_18))
		{
			ATP_test_number = 18;
		}
		else if (strstr(command_str, test_nbr_19))
		{
			ATP_test_number = 19;
		}
		else if (strstr(command_str, test_nbr_20))
		{
			ATP_test_number = 20;
		}
		else if (strstr(command_str, test_nbr_21))
		{
			ATP_test_number = 21;
		}
		else if (strstr(command_str, test_nbr_22))
		{
			ATP_test_number = 22;
		}
		else if (strstr(command_str, test_nbr_23))
		{
			ATP_test_number = 23;
		}
		else if (strstr(command_str, test_nbr_24))
		{
			ATP_test_number = 24;
		}
		else if (strstr(command_str, test_nbr_25))
		{
			ATP_test_number = 25;
		}
		else
		{
			ATP_test_number = 0 ;
		}

		memset(command_str,0,sizeof(command_str));

		switch(ATP_test_number)
		{
		case  POWER_CTRL_1 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Before Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOA,Pwr_Ctrl1_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			HAL_GPIO_TogglePin(GPIOA,Pwr_Ctrl1_Pin);
			HAL_Delay(5);
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"After Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOA,Pwr_Ctrl1_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  POWER_CTRL_2 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Before Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOA,Pwr_Ctrl2_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			HAL_GPIO_TogglePin(GPIOA,Pwr_Ctrl2_Pin);
			HAL_Delay(5);
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"After Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOA,Pwr_Ctrl2_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  INPUT_GPIO_1 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOD,GPI_1_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  INPUT_GPIO_2 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOD,GPI_2_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  INPUT_GPIO_3 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOE,GPI_3_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  INPUT_GPIO_4 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOE,GPI_4_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  INPUT_GPIO_5 		:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOE,GPI_5_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  OUTPUT_GPIO_1 	:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Before Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOB,GPO_1_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			HAL_GPIO_TogglePin(GPIOB,GPO_1_Pin);
			HAL_Delay(5);
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"After Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOB,GPO_1_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  OUTPUT_GPIO_2 	:
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"Before Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOB,GPO_2_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			HAL_GPIO_TogglePin(GPIOB,GPO_2_Pin);
			HAL_Delay(5);
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			sprintf(ATP_buffer,"After Pin State %d\n\r",HAL_GPIO_ReadPin(GPIOB,GPO_2_Pin));
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, strlen(ATP_buffer));
			break;
		case  TS_1		 		:
			ADC_READ( 1, 1);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",1,1,temp_sensor_1);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_2		 		:
			ADC_READ( 1, 2);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",1,2,temp_sensor_2);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_3		 		:
			ADC_READ( 1, 3);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",1,3,temp_sensor_3);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_4		 		:
			ADC_READ( 1, 4);			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",1,4,temp_sensor_4);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));			HAL_Delay(50);
			break;
		case  TS_5		 		:
			ADC_READ( 2, 1);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",2,1,temp_sensor_5);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_6		 		:
			ADC_READ( 2, 2);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",2,2,temp_sensor_6);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_7		 		:
			ADC_READ( 2, 3);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",2,3,temp_sensor_7);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_8		 		:
			ADC_READ( 2, 4);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",2,4,temp_sensor_8);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_9		 		:
			ADC_READ( 3, 1);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",3,1,temp_sensor_9);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			break;

		case  TS_10		 		:
			ADC_READ( 3, 2);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",3,2,temp_sensor_10);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_11		 		:
			ADC_READ( 3, 3);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",3,3,temp_sensor_11);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_12		 		:
			ADC_READ( 3, 4);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d   temp = %f\n\r",3,4,temp_sensor_12);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  RTC_CHECK	 		:
			//HAL_Delay(500);
			uint8_t time_data_1[8]={0};
			uint32_t sec_addr = 0x60000000 | 0x7FFF9;
			//			while(1)
			//			{
			//HAL_Delay(400);
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			//			while(time_data_1[6]== 0)
			//			{
			HAL_Delay(100);
			HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&time_data_1,7);
			//}
			if(time_data_1[6]!= 0)
			{
				sprintf(ATP_buffer, "{\"TIME\": \"%x:%x:%x\",\"DATE\": \"%x-%x-20%x}\"\n\r",time_data_1[2],time_data_1[1],time_data_1[0],time_data_1[4],time_data_1[5],time_data_1[6]);
				HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
				HAL_Delay(100);
			}
			else
			{
				ATP_test = 1;
				command_str[0]='T';
				command_str[1] = '2';
				command_str[2] = '2';
				command_str[3]= 'E';
			}

			//}
			break;
		case  NAND_FLASH_WRITE	:

			break;
		case  NAND_FLASH_READ	:

			break;
		case  NAND_FLASH_ERASE	:

			break;
		}

	}



}
