/*
 * SRAM_Operations.c
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
/* USER CODE END Includes */

#define NAND_PAGE_SIZE 8192

extern SRAM_HandleTypeDef hsram1;
extern UART_HandleTypeDef huart5;

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

uint8_t data[8192] = {0};
uint32_t sram_location = 0x60000010;
uint32_t sram_base_location = 0x60000010;


uint32_t sramAddr_base_addr = 0x60000000 ;
uint32_t sec_addr = 0x60000000 | 0x7FFF9;
uint32_t min_addr = 0x60000000 | 0x7FFFa;
uint32_t hr_addr = 0x60000000 | 0x7FFFb;
uint32_t day_addr = 0x60000000 | 0x7FFFc;
uint32_t date_addr = 0x60000000 | 0x7FFFd;
uint32_t month_addr = 0x60000000 | 0x7FFFe;
uint32_t year_addr = 0x60000000 | 0x7FFFf;


extern NAND_AddressTypeDef nand_address;
extern uint8_t time_set;
extern void Erase_SRAM(SRAM_HandleTypeDef *hsram1);
extern uint8_t time_str[25];
extern uint32_t sramAddr ;

uint32_t page_addr = 0x60000000;
uint32_t block_addr = 0x60000002; //plane addr
uint32_t plane_addr = 0x60000004;


uint32_t page = 0;
uint32_t block = 0;
uint32_t plane = 0;


uint32_t size_counter_addr = 0x6000000C;



extern uint8_t nand_update ;


uint8_t pDstBuffer = 0;
uint8_t pSrcBuffer = 0x80;
uint32_t BufferSize = 1;
uint8_t sram_buffer[25]={0};
uint8_t dataToWrite[256];



uint8_t SRAM_buffer[300]={0};

uint16_t size_counter = 0 ;

uint8_t old_sec = 0 ;
uint8_t cur_sec = 0 ;





void time_setter( );
void update_NAND_addr();

uint8_t SRAM_operation()
{

	uint8_t time_data[8]={0};
//	Erase_SRAM(&hsram1);
	//HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&cur_sec,1);
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&time_data,7);

	if(time_data[0] != old_sec)
	{
		memset(SRAM_buffer,0,sizeof(SRAM_buffer));
		sprintf(SRAM_buffer,
				"{\"TS1\": \"%.2f\", \"TS2\": \"%.2f\", \"TS3\": \"%.2f\", \"TS4\": \"%.2f\", \"TS5\": \"%.2f\", "
				"\"TS6\": \"%.2f\", \"TS7\": \"%.2f\", \"TS8\": \"%.2f\", \"TS9\": \"%.2f\", \"TS10\": \"%.2f\", "
				"\"TS11\": \"%.2f\", \"TS12\": \"%.2f\", \"TIME\": \"%x:%x:%x\",\"DATE\": \"%x-%x-20%x}\"\n\r",
				temp_sensor_1, temp_sensor_2, temp_sensor_3, temp_sensor_4, temp_sensor_5, temp_sensor_6,
				temp_sensor_7, temp_sensor_8, temp_sensor_9, temp_sensor_10, temp_sensor_11, temp_sensor_12,
				time_data[2],time_data[1],time_data[0],time_data[4],time_data[5],time_data[6]);
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_location, (uint8_t *)&SRAM_buffer, strlen(SRAM_buffer));
		sram_location = sram_location + strlen(SRAM_buffer);
		size_counter += strlen(SRAM_buffer);
		if(size_counter + strlen(SRAM_buffer) > NAND_PAGE_SIZE)
		{
			sram_location = sram_base_location ;
			size_counter = 0 ;
			nand_update = 1;
			update_NAND_addr();

		}

		old_sec = time_data[0];

	}

	if(time_set)
	{
		time_set = 0 ;
		time_setter();

	}

}


void get_NAND_data(uint8_t *sram_data)
{
	uint8_t CLEAN_DATA[8192] = {0};
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_base_location, sram_data, NAND_PAGE_SIZE);
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_base_location, CLEAN_DATA, NAND_PAGE_SIZE);
	//HAL_UART_Transmit_IT(&huart8, sram_data, strlen(sram_data));
//	HAL_Delay(100);
}

uint8_t get_NAND_address(void)
{

	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sramAddr_base_addr,(uint8_t *)&nand_address, sizeof(nand_address));

}


void update_NAND_addr()
{
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr_base_addr, (uint8_t * )&nand_address, sizeof(nand_address));


}



void time_setter( )
{
	//	0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
	//	S D D - M M - Y Y Y  Y     H  H  :  M  M  :  S  S  E
	uint8_t day, month, year, hours, minutes, seconds;
	if (time_str[0] == 'S' && time_str[20] == 'E' && time_str[5] == '-' && time_str[8] == '-' && time_str[11] == ' ' && time_str[14] == ':' && time_str[17] == ':') // && time_str[11] == ' ' &&
	{
		month = (time_str[6] - 48) * 16 + (time_str[7] - 48);
		year = (time_str[3] - 48) * 16 + (time_str[4] - 48);
		day = (time_str[9] - 48) * 16 + (time_str[10] - 48);
		minutes = (time_str[15] - 48) * 16 + (time_str[16] - 48);
		hours = (time_str[12] - 48) * 16 + (time_str[13] - 48);
		seconds = (time_str[18] - 48) * 16 + (time_str[19] - 48);


		sramAddr = 0x60000000 | 0x7FFF8;
		pSrcBuffer = 0x80;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);


		sramAddr = 0x60000000 | 0x7FFF9;
		pSrcBuffer = seconds;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);

		sramAddr = 0x60000000 | 0x7FFFa;
		pSrcBuffer =  minutes;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);

		sramAddr = 0x60000000 | 0x7FFFb;
		pSrcBuffer =  hours;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);

		sramAddr = 0x60000000 | 0x7FFFc;
		pSrcBuffer = 0x02;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);

		sramAddr = 0x60000000 | 0x7FFFd;
		pSrcBuffer =  day;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);

		sramAddr = 0x60000000 | 0x7FFFe;
		pSrcBuffer =   month;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);

		sramAddr = 0x60000000 | 0x7FFFf;
		pSrcBuffer =  year ;
		HAL_SRAM_Write_8b(&hsram1,(uint32_t *)sramAddr,&pSrcBuffer,BufferSize);


	    sramAddr = 0x60000000 | 0x7FFF8;
	    pSrcBuffer = 0x00;
	    HAL_SRAM_Write_8b(&hsram1,sramAddr,&pSrcBuffer,BufferSize);

	}

}
