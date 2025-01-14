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
#include "ATP_Mode.h"


char *test_nbr_1 = "01";
char *test_nbr_2 = "02";
char *test_nbr_3 = "03";
char *test_nbr_4 = "04";
char *test_nbr_5 = "05";
char *test_nbr_6 = "06";
char *test_nbr_7 = "07";
char *test_nbr_8 = "08";
char *test_nbr_9 = "09";
char *test_nbr_10 = "10";
char *test_nbr_11 = "11";
char *test_nbr_12 = "12";
char *test_nbr_13 = "13";
char *test_nbr_14 = "14";
char *test_nbr_15 = "15";
char *test_nbr_16 = "16";
char *test_nbr_17 = "17";
char *test_nbr_18 = "18";
char *test_nbr_19 = "19";
char *test_nbr_20 = "20";
char *test_nbr_21 = "21";
char *test_nbr_22 = "22";
char *test_nbr_23 = "23";
char *test_nbr_24 = "24";
char *test_nbr_25 = "25";
char *test_nbr_26 = "26";
char *test_nbr_27 = "27";
char *test_nbr_28 = "28";
char *test_nbr_29 = "29";

// Variables for storing temperature sensor readings
extern float temp_sensor[3][4] ;;

// Variables for storing temperature sensor readings
extern uint16_t sensor_hex[3][4] ;

char command_str[20]={0};
uint8_t command_str_index = 0;
uint8_t ATP_test = 0 ;
uint8_t ATP_test_number = 0 ;
uint8_t LOOPBACK_BUF[20]={0};
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
			ATP_test_number = POWER_CTRL_1;
		}
		else if (strstr(command_str, test_nbr_2))
		{
			ATP_test_number = POWER_CTRL_2;
		}
		else if (strstr(command_str, test_nbr_3))
		{
			ATP_test_number = INPUT_GPIO_1;
		}
		else if (strstr(command_str, test_nbr_4))
		{
			ATP_test_number = INPUT_GPIO_2;
		}
		else if (strstr(command_str, test_nbr_5))
		{
			ATP_test_number = INPUT_GPIO_3;
		}
		else if (strstr(command_str, test_nbr_6))
		{
			ATP_test_number = INPUT_GPIO_4;
		}
		else if (strstr(command_str, test_nbr_7))
		{
			ATP_test_number = INPUT_GPIO_5;
		}
		else if (strstr(command_str, test_nbr_8))
		{
			ATP_test_number = OUTPUT_GPIO_1;
		}
		else if (strstr(command_str, test_nbr_9))
		{
			ATP_test_number = OUTPUT_GPIO_2;
		}
		else if (strstr(command_str, test_nbr_10))
		{
			ATP_test_number = TS_1;
		}
		else if (strstr(command_str, test_nbr_11))
		{
			ATP_test_number = TS_2;
		}
		else if (strstr(command_str, test_nbr_12))
		{
			ATP_test_number = TS_3;
		}
		else if (strstr(command_str, test_nbr_13))
		{
			ATP_test_number = TS_4;
		}
		else if (strstr(command_str, test_nbr_14))
		{
			ATP_test_number = TS_5;
		}
		else if (strstr(command_str, test_nbr_15))
		{
			ATP_test_number = TS_6;
		}
		else if (strstr(command_str, test_nbr_16))
		{
			ATP_test_number = TS_7;
		}
		else if (strstr(command_str, test_nbr_17))
		{
			ATP_test_number = TS_8;
		}
		else if (strstr(command_str, test_nbr_18))
		{
			ATP_test_number = TS_9;
		}
		else if (strstr(command_str, test_nbr_19))
		{
			ATP_test_number = TS_10;
		}
		else if (strstr(command_str, test_nbr_20))
		{
			ATP_test_number = TS_11;
		}
		else if (strstr(command_str, test_nbr_21))
		{
			ATP_test_number = TS_12;
		}
		else if (strstr(command_str, test_nbr_22))
		{
			ATP_test_number = RTC_CHECK;
		}
		else if (strstr(command_str, test_nbr_23))
		{
			ATP_test_number = NAND_FLASH_WRITE;
		}
		else if (strstr(command_str, test_nbr_24))
		{
			ATP_test_number = NAND_FLASH_READ;
		}
		else if (strstr(command_str, test_nbr_25))
		{
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)== HIGH)
				ATP_test_number = NAND_FLASH_ERASE;
			else{
				HAL_UART_Transmit_IT(&huart5, "Is GPIO HIGH..?", strlen("Is GPIO HIGH..?"));
				HAL_Delay(100);
			}
		}
		else if (strstr((char *)command_str, test_nbr_26))
		{
			//			memset(LOOPBACK_BUF,0,sizeof(LOOPBACK_BUF));
			memcpy(LOOPBACK_BUF,&command_str[3],(strlen((char *)(command_str))-4));
			ATP_test_number = LoopBack_RS232;
		}
		else if (strstr((char *)command_str, test_nbr_27))
		{
			//			memset(LOOPBACK_BUF,0,sizeof(LOOPBACK_BUF));
			memcpy(LOOPBACK_BUF,&command_str[3],(strlen((char *)(command_str))-4));
			ATP_test_number = LoopBack_RS422;
		}
		else if (strstr((char *)command_str, test_nbr_28))
		{
			memcpy(LOOPBACK_BUF,&command_str[3],(strlen((char *)(command_str))-4));
			ATP_test_number = LoopBack1_RS485;
		}
		else if (strstr((char *)command_str, test_nbr_29))
		{
			memcpy(LOOPBACK_BUF,&command_str[3],(strlen((char *)(command_str))-4));
			ATP_test_number = LoopBack2_RS485;
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
			ADC_READ( adc_1, channel_1);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",1,1,sensor_hex[0][0],temp_sensor[0][0]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_2		 		:
			ADC_READ( adc_1, channel_2);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",1,2,sensor_hex[0][1],temp_sensor[0][1]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_3		 		:
			ADC_READ( adc_1, channel_3);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",1,3,sensor_hex[0][2],temp_sensor[0][2]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_4		 		:
			ADC_READ( adc_1, channel_4);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d  0x%X  temp = %f\n\r",1,4,sensor_hex[0][3],temp_sensor[0][3]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_5		 		:
			ADC_READ( adc_2, channel_1);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0X%X    temp = %f\n\r",2,1,sensor_hex[1][0],temp_sensor[1][0]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_6		 		:
			ADC_READ( adc_2, channel_2);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",2,2,sensor_hex[1][1],temp_sensor[1][1]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_7		 		:
			ADC_READ( adc_2, channel_3);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",2,3,sensor_hex[1][2],temp_sensor[1][2]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_8		 		:
			ADC_READ( adc_2, channel_4);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d  0x%X  temp = %f\n\r",2,4,sensor_hex[1][3],temp_sensor[1][3]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_9		 		:
			ADC_READ( adc_3, channel_1);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X   temp = %f\n\r",3,1,sensor_hex[2][0],temp_sensor[2][0]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			break;

		case  TS_10		 		:
			ADC_READ( adc_3, channel_2);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d  0x%X  temp = %f\n\r",3,2,sensor_hex[2][1],temp_sensor[2][1]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_11		 		:
			ADC_READ( adc_3, channel_3);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",3,3,sensor_hex[2][2],temp_sensor[2][2]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  TS_12		 		:
			ADC_READ( adc_3, channel_4);
			HAL_Delay(50);
			sprintf(ATP_buffer, "ADC    %d  Sensor %d 0x%X  temp = %f\n\r",3,4,sensor_hex[2][3],temp_sensor[2][3]);
			HAL_UART_Transmit_IT(&huart5, ATP_buffer, sizeof(ATP_buffer));
			HAL_Delay(50);
			break;
		case  RTC_CHECK	 		:
			uint8_t time_data_1[8]={0};
			uint32_t sec_addr = 0x60000000 | 0x7FFF9;
			memset(ATP_buffer,0,sizeof(ATP_buffer));
			HAL_Delay(100);
			HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&time_data_1,7);

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
			break;
		case  NAND_FLASH_WRITE	:

			break;

		case  NAND_FLASH_READ	:
			get_NAND_stored_data();
			break;

		case  NAND_FLASH_ERASE	:
			if (HAL_NAND_Erase_Block(&hnand1,&nand_address) == HAL_OK)
			{
				HAL_UART_Transmit_IT(&huart5, "NAND erased successfully!\n", sizeof("NAND erased successfully!\n"));
				HAL_Delay(100);
			}
			else
			{
				HAL_UART_Transmit_IT(&huart5, "Error erasing NAND.\n", sizeof("Error erasing NAND.\n"));
				HAL_Delay(100);
			}
			break;

		case LoopBack_RS232: //RS-232
			HAL_UART_Transmit_IT(&huart5, LOOPBACK_BUF, strlen((char *)LOOPBACK_BUF));
			HAL_Delay(100);
			memset(LOOPBACK_BUF,0,sizeof(LOOPBACK_BUF));

			break;
		case LoopBack_RS422: //RS-422
			HAL_GPIO_WritePin(GPIOA, RS422_CTRL_Pin,  GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_UART_Transmit_IT(&huart8, LOOPBACK_BUF, strlen((char *)LOOPBACK_BUF));
			HAL_Delay(100);
			memset(LOOPBACK_BUF,0,sizeof(LOOPBACK_BUF));
			HAL_GPIO_WritePin(GPIOA, RS422_CTRL_Pin,  GPIO_PIN_RESET);

			break;
		case LoopBack1_RS485: //RS-485
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL1_Pin, 1);
			HAL_UART_Transmit_IT(&huart10, LOOPBACK_BUF, strlen((char *)LOOPBACK_BUF));
			HAL_Delay(100);
			memset(LOOPBACK_BUF,0,sizeof(LOOPBACK_BUF));
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL1_Pin, 0);

			break;
		case LoopBack2_RS485: //RS-485
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL2_Pin, 1);
			HAL_UART_Transmit_IT(&huart4, LOOPBACK_BUF, strlen((char *)LOOPBACK_BUF));
			HAL_Delay(100);
			memset(LOOPBACK_BUF,0,sizeof(LOOPBACK_BUF));
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL2_Pin, 0);

			break;

		}

	}

}
