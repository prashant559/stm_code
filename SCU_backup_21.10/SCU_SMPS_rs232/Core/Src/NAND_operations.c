/*
 * NAND_operations.c
 *
 *  Created on: Sep 9, 2024
 *      Author: VIMAL
 */


/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>

extern NAND_HandleTypeDef hnand1;
extern UART_HandleTypeDef huart5;
/* USER CODE END Includes */
NAND_IDTypeDef 		nand_details;
NAND_AddressTypeDef nand_address = {0,0,0};
//	NAND_DeviceConfigTypeDef pDeviceConfig;

uint8_t read_data[8195] = { 0 };

uint8_t data_write = 100 ;




uint32_t page_read = 0 ;
uint32_t block_read = 0 ;
uint32_t plane_read = 0 ;
uint8_t once = 1;

uint8_t nand_update = 0 ;


uint8_t NAND_data[8192] = {0};


void NAND_operation(void)
{
	HAL_GPIO_WritePin(GPIOE, LED1_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOE, LED1_Pin, 0);
	HAL_Delay(1);

	if(nand_update)
	{
		//		HAL_NAND_Erase_Block(&hnand1, &nand_address);
		//		HAL_Delay(1000);
		memset(&read_data[0],0,sizeof(read_data));
		nand_update = 0 ;
		get_NAND_data(NAND_data);
		//		while(1)
		//		{
		HAL_NAND_Write_Page_8b(&hnand1, &nand_address,&NAND_data, 1);
		HAL_UART_Transmit_IT(&huart5, "\n\rNAND_WRITE_DATA\n\r", strlen("NAND_READ_DATA\n\r"));
		HAL_Delay(10);
		HAL_UART_Transmit_IT(&huart5, NAND_data, 8192);
		HAL_Delay(1000);


		HAL_NAND_Read_ID(&hnand1,&nand_details); //????
		HAL_NAND_Read_ID(&hnand1,&read_data); //????
		//HAL_Delay(100);
		HAL_NAND_Read_Page_8b(&hnand1,(const NAND_AddressTypeDef  *) &nand_address,(uint8_t *)&read_data, 1);
		HAL_UART_Transmit_IT(&huart5, "\n\rNAND_READ_DATA\n\r", strlen("NAND_READ_DATA\n\r"));
		HAL_Delay(10);
		HAL_UART_Transmit_IT(&huart5, read_data, 8192);
		HAL_Delay(1000);
		nand_address.Page++;

		if(NAND_data[4500] != read_data[4500]) // page size 8192bytes
		{
//			nand_address.Page = 0;
//			HAL_NAND_Erase_Block(&hnand1, &nand_address);
//			HAL_Delay(1000);
		}
		if(nand_address.Page> 127) //1block =128 pages
		{
			nand_address.Block ++;//= ++block_read;
			nand_address.Page = 0;
			HAL_NAND_Erase_Block(&hnand1, &nand_address);
			HAL_Delay(1000);
		}
		//		}
	}

}
