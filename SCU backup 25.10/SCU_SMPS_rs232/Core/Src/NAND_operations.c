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
#include "Global.h"

extern NAND_HandleTypeDef hnand1;
extern UART_HandleTypeDef huart5;
extern void get_NAND_data(uint8_t *);
_sNandOperation Nandopt;
/* USER CODE END Includes */
NAND_IDTypeDef 		nand_details;
NAND_AddressTypeDef nand_address = {0,0,0};

//	NAND_DeviceConfigTypeDef pDeviceConfig;
//uint8_t read_data[8195] = { 0 };
//uint8_t NAND_data[8192] = {0};
//uint8_t data_write = 100 ;

uint32_t page_read = 0 ;
uint32_t block_read = 0 ;
uint32_t plane_read = 0 ;
uint8_t once = 1;

void NAND_operation(void);

void NAND_operation(void)
{
	HAL_GPIO_WritePin(GPIOE, LED1_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOE, LED1_Pin, 0);
	HAL_Delay(1);

	if(Nandopt.Nand_flag)//nand_update
	{
		//		HAL_NAND_Erase_Block(&hnand1, &nand_address);
		//		HAL_Delay(1000);
		Nandopt.Nand_flag = LOW ;
		memset(&Nandopt.NAND_read[0],0,sizeof(Nandopt.NAND_read));
		get_NAND_data(&Nandopt.NAND_write[0]);
		//		while(1)
		//		{
		HAL_NAND_Write_Page_8b(&hnand1, &nand_address,&Nandopt.NAND_write[0], 1);
#ifdef NandUART
		HAL_UART_Transmit_IT(&huart5, "\n\rNAND_WRITE_DATA\n\r", strlen("NAND_READ_DATA\n\r"));
		HAL_Delay(10);
		HAL_UART_Transmit_IT(&huart5, &Nandopt.NAND_write[0], 8192);
		HAL_Delay(1000);
#endif
		HAL_NAND_Read_ID(&hnand1,&nand_details); //????
		HAL_NAND_Read_ID(&hnand1,(NAND_IDTypeDef *)&Nandopt.NAND_read[0]); //????
		//HAL_Delay(100);
		HAL_NAND_Read_Page_8b(&hnand1,(const NAND_AddressTypeDef  *) &nand_address,(uint8_t *)&Nandopt.NAND_read[0], 1);
#ifdef NandUART
		HAL_UART_Transmit_IT(&huart5, "\n\rNAND_READ_DATA\n\r", strlen("NAND_READ_DATA\n\r"));
		HAL_Delay(10);
		HAL_UART_Transmit_IT(&huart5, &Nandopt.NAND_read[0], 8192);
		HAL_Delay(1000);
#endif
		nand_address.Page++;

		if(Nandopt.NAND_write[4500] != Nandopt.NAND_read[4500]) // page size 8192bytes
		{
			nand_address.Page = 0;
			HAL_NAND_Erase_Block(&hnand1, &nand_address);
			HAL_Delay(1000);
		}
		if(nand_address.Page> 127) //1block =128 pages
		{
			nand_address.Block ++;
			nand_address.Page  = 0;
			HAL_NAND_Erase_Block(&hnand1, &nand_address);
			HAL_Delay(1000);
		}
		if(nand_address.Block> 2047)
		{
			nand_address.Plane ++;
			nand_address.Block =0;
			HAL_NAND_Erase_Block(&hnand1, &nand_address);
			HAL_Delay(1000);
		}
		//		}
	}

}
