/*
 * SRAM_Operations.c
 *
 *  Created on: Sep 9, 2024
 *      Author: VIMAL
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "ATP_Mode.h"
#include "SRAM_Operations.h"

#define NAND_PAGE_SIZE 8192  // Define the NAND page size

/* External variables --------------------------------------------------------*/
extern SRAM_HandleTypeDef hsram1;  // Handle for SRAM
extern UART_HandleTypeDef huart5;  // Handle for UART
extern UART_HandleTypeDef huart4;  // Handle for UART
extern UART_HandleTypeDef huart8;  // Handle for UART
extern UART_HandleTypeDef huart10;  // Handle for UART

// Temperature sensor variables
extern float temp_sensor[3][4];



extern uint16_t sensor_hex[3][4];

// SRAM and NAND addresses
uint8_t data[8192] = {0};
uint32_t sram_location = 0x60000010;
uint32_t sram_base_location = 0x60000010;
uint32_t sramAddr_base_addr = 0x60000000;
uint32_t sec_addr = 0x60000000 | 0x7FFF9;
uint32_t min_addr = 0x60000000 | 0x7FFFa;
uint32_t hr_addr = 0x60000000 | 0x7FFFb;
uint32_t day_addr = 0x60000000 | 0x7FFFc;
uint32_t date_addr = 0x60000000 | 0x7FFFd;
uint32_t month_addr = 0x60000000 | 0x7FFFe;
uint32_t year_addr = 0x60000000 | 0x7FFFf;

extern NAND_AddressTypeDef nand_address;  // NAND address structure
extern uint8_t time_set;  // Flag for setting time
extern uint8_t time_str[25];  // Time string
extern uint32_t sramAddr;  // SRAM address

_sSRAMOperation SRAMopt;
uint32_t page_addr = 0x60000000;
uint32_t block_addr = 0x60000002;
uint32_t plane_addr = 0x60000004;
uint32_t size_counter_addr = 0x6000000C;

// Buffers for SRAM operations
uint8_t SRAM_buffer[300] = {0};
uint8_t pDstBuffer = 0;
uint8_t pSrcBuffer = 0x80;
uint32_t BufferSize = 1;
uint8_t sram_buffer[25] = {0};
uint8_t dataToWrite[256];
uint16_t size_counter = 0;
uint8_t old_sec = 0;
uint8_t cur_sec = 0;
uint8_t counter =0;

extern uint8_t check_channel ;
extern uint8_t nand_update;  // Flag for NAND update

/* Function Prototypes -------------------------------------------------------*/
void time_setter();
void update_NAND_addr();

/*
 * SRAM_operation
 * Reads time data from SRAM, updates temperature readings, and writes formatted
 * data back to SRAM. Checks for time changes and triggers NAND update if necessary.
 */
uint8_t SRAM_operation() {
	uint8_t time_data[8] = {0};
	uint16_t bufSize = 0;

	// Read current second from SRAM
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&time_data, 7);

	// Check if the second has changed
	if (time_data[0] != old_sec ) {
		HAL_GPIO_WritePin(GPIOE, LED4_Pin, 1); // Toggle LED for visual feedback
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOE, LED4_Pin, 0);
		//		memset(SRAM_buffer, 0, sizeof(SRAM_buffer));

		// Format temperature and time data into the buffer
		memset(SRAMopt.SRAM_buffer,0,sizeof(SRAMopt.SRAM_buffer));
		SRAMopt.Startbyte=SoF;
		SRAMopt.Idxcount=sprintf((char *)SRAMopt.SRAM_buffer,
				"{\"TS1\": \"%.2f#%X\", \"TS2\": \"%.2f#%X\", \"TS3\": \"%.2f#%X\", \"TS4\": \"%.2f#%X\", \"TS5\": \"%.2f#%X\", "
				"\"TS6\": \"%.2f#%X\", \"TS7\": \"%.2f#%X\", \"TS8\": \"%.2f#%X\", \"TS9\": \"%.2f#%X\", \"TS10\": \"%.2f#%X\", "
				"\"TS11\": \"%.2f#%X\", \"TS12\": \"%.2f#%X\", \"TIME\": \"%x:%x:%x\",\"DATE\": \"%x-%x-20%x\"} \n\r",
				temp_sensor[0][0],sensor_hex[0][0] ,temp_sensor[0][1],sensor_hex[0][1], temp_sensor[0][2],sensor_hex[0][2], temp_sensor[0][3],sensor_hex[0][3], temp_sensor[1][0],sensor_hex[1][0], temp_sensor[1][1],sensor_hex[1][1],
				temp_sensor[1][2],sensor_hex[1][2], temp_sensor[1][3],sensor_hex[1][3], temp_sensor[2][0],sensor_hex[2][0], temp_sensor[2][1],sensor_hex[2][1], temp_sensor[2][2],sensor_hex[2][2], temp_sensor[2][3],sensor_hex[2][3],
				time_data[2], time_data[1], time_data[0], time_data[4], time_data[5], time_data[6]);
		SRAMopt.SRAM_buffer[(SRAMopt.Idxcount)-3]=EoF;

		// Write the buffer to SRAM
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_location, (uint8_t *)&SRAMopt,((SRAMopt.Idxcount)+1));
		HAL_Delay(50);

		// Update SRAM location and size counter
		sram_location += ((SRAMopt.Idxcount)+1);
		size_counter  += ((SRAMopt.Idxcount)+1);
		counter++;

		if(!ATP_UART)
		{

			HAL_GPIO_WritePin(GPIOA, RS485_CTRL1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOA, RS422_CTRL_Pin,  GPIO_PIN_SET);
			HAL_Delay(2);
			HAL_UART_Transmit_IT(&huart8, (uint8_t *)&SRAMopt, (SRAMopt.Idxcount)+1);
			HAL_UART_Transmit_IT(&huart10,(uint8_t *)&SRAMopt, (SRAMopt.Idxcount)+1);
			HAL_UART_Transmit_IT(&huart4, (uint8_t *)&SRAMopt, (SRAMopt.Idxcount)+1);
			HAL_UART_Transmit_IT(&huart5, (uint8_t *)&SRAMopt, (SRAMopt.Idxcount)+1);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOA, RS422_CTRL_Pin,  GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOA, RS485_CTRL2_Pin, GPIO_PIN_RESET);
		}

		// Check if the buffer exceeds the NAND page size
		if (size_counter + (SRAMopt.Idxcount) > NAND_PAGE_SIZE) {
			sram_location = sram_base_location;
			bufSize = 0;
			size_counter = 0;
			counter=0;
			nand_update = 1;
		}

		// Update old_sec to current second
		old_sec = time_data[0];
	}

	// If time setting flag is set, call time_setter
	if (time_set) {
		time_set = 0;
		time_setter();
	}
	return 0;
}

/*
 * get_NAND_data
 * Reads a full page of data from SRAM into sram_data buffer and clears SRAM.
 */
void get_NAND_data(uint8_t *sram_data) {
	uint8_t CLEAN_DATA[8192] = {0};

	// Read NAND page from SRAM
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sram_base_location, sram_data, NAND_PAGE_SIZE);

	// Clear the SRAM location
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sram_base_location, CLEAN_DATA, NAND_PAGE_SIZE);
}

/*
 * get_NAND_address
 * Reads the current NAND address from SRAM.
 */
uint8_t get_NAND_address(void) {
	HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sramAddr_base_addr + 2, (uint8_t *)&nand_address, sizeof(nand_address));
	if(nand_address.Page >= 128)
		nand_address.Page=0;
	if(nand_address.Block >= 4096)
		nand_address.Block=0;
	if(nand_address.Plane > 2)
		nand_address.Plane=0;

	return 0;
}

/*
 * update_NAND_addr
 * Writes the current NAND address to SRAM.
 */
void update_NAND_addr() {
	HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr_base_addr + 2, (uint8_t *)&nand_address, sizeof(nand_address));
}

/*
 * time_setter
 * Sets the time in SRAM using the format "SDD-MM-YYYY HH:MM:SS E".
 */
void time_setter() {
	uint8_t day, month, year, hours, minutes, seconds;
	//	0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
	//	S D D - M M - Y Y Y  Y     H  H  :  M  M  :  S  S  E
	// Parse the time string if it follows the expected format
	if (time_str[0] == 'S' && time_str[20] == 'E' &&
			time_str[5] == '-' && time_str[8] == '-' &&
			time_str[11] == ' ' && time_str[14] == ':' && time_str[17] == ':') {

		// Extract date and time components from time_str
		month = (time_str[6] - '0') * 16 + (time_str[7] - '0');
		year = (time_str[3] - '0') * 16 + (time_str[4] - '0');
		day = (time_str[9] - '0') * 16 + (time_str[10] - '0');
		minutes = (time_str[15] - '0') * 16 + (time_str[16] - '0');
		hours = (time_str[12] - '0') * 16 + (time_str[13] - '0');
		seconds = (time_str[18] - '0') * 16 + (time_str[19] - '0');

		// Write time components to SRAM
		sramAddr = 0x60000000 | 0x7FFF8;
		pSrcBuffer = 0x80;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFF9;
		pSrcBuffer = seconds;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFFa;
		pSrcBuffer = minutes;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFFb;
		pSrcBuffer = hours;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFFc;
		pSrcBuffer = 0x02;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFFd;
		pSrcBuffer = day;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFFe;
		pSrcBuffer = month;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFFf;
		pSrcBuffer = year;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);

		sramAddr = 0x60000000 | 0x7FFF8;
		pSrcBuffer = 0x00;
		HAL_SRAM_Write_8b(&hsram1, (uint32_t *)sramAddr, &pSrcBuffer, BufferSize);
	}
}


