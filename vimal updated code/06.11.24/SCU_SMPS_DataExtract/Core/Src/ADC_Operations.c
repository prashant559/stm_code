/*
 * ADC_Operations.c
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
#include "ADC_Operations.h"
// Function prototype for reading ADC values
uint16_t ADC_READ(uint8_t adc_num, uint8_t channel);
uint8_t check_sensor_validity(uint16_t data);
uint8_t wire_break(uint8_t flag);
// External SPI and SRAM handles, needed for SPI communication and SRAM operations
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi5;
extern SRAM_HandleTypeDef hsram1;

// External UART handles for UART communication
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart10;

// ADC register addresses
const uint8_t ADC_ID = 0x00;
const uint8_t ADC_STATUS = 0x01;
const uint8_t ADC_INPMUX = 0x02;
const uint8_t ADC_PGA = 0x03;
const uint8_t ADC_DATARATE = 0x04;
const uint8_t ADC_REF = 0x05;
const uint8_t ADC_IDACMAG = 0x06;
const uint8_t ADC_IDACMUX = 0x07;
const uint8_t ADC_VBIAS = 0x08;
const uint8_t ADC_SYS = 0x09;
const uint8_t ADC_RESERVED_0 = 0x0A;
const uint8_t ADC_OFCAL0 = 0x0B;
const uint8_t ADC_OFCAL1 = 0x0C;
const uint8_t ADC_RESERVED_1 = 0x0D;
const uint8_t ADC_FSCAL0 = 0x0E;
const uint8_t ADC_FSCAL1 = 0x0F;
const uint8_t ADC_GPIODAT = 0x10;
const uint8_t ADC_GPIOCON = 0x11;
uint8_t txd_buf[30]={0};
// ADC command constants
const uint8_t CMD_NOP = 0x00;
const uint8_t CMD_WAKEUP = 0x02;
const uint8_t CMD_POWERDN = 0x04;
const uint8_t CMD_RESET = 0x06;
const uint8_t CMD_START = 0x08;
const uint8_t CMD_STOP = 0x0A;
const uint8_t CMD_SYOCAL = 0x16;
const uint8_t CMD_SYGCAL = 0x17;
const uint8_t CMD_SFOCAL = 0x19;
const uint8_t CMD_RDATA = 0x13;
const uint8_t CMD_RREG = 0x20;
const uint8_t CMD_WREG = 0x40;


#define ADC_1 0 
#define ADC_2 1 
#define ADC_3 2 


// Buffers for communication and data storage
uint8_t buffer[300] = {0};
uint8_t buffer_1[300] = {0};
uint32_t count = 0;  // Counter variable


uint8_t sensor_table[3][4]={0};
uint16_t Error_table[]={
		0x7FFF,
		0x8000,
		0xE005,
		0x200B,
		0x200D,
		0x1FFC,
		0x1FF2,
		0xE00F,
		0xE00D,
		0x1FF4,
		0xDFF4,
		0xDFF6,
		0xFFCD,
		0xFED0,
		0xDFFB,
		0x2012
};


// Transmission and reception buffers for each ADC channel
uint8_t txd_buf_1[30] = {0}, txd_buf_2[30] = {0}, txd_buf_3[30] = {0};
uint8_t rxd_buf_1[30] = {0}, rxd_buf_2[30] = {0}, rxd_buf_3[30] = {0};

// Variables to store temperature and resistance readings
float temperature = 0.0, resistance = 0.0;
uint16_t reg_data_ADC_1[10] = {0}, reg_data_ADC_2[10] = {0}, reg_data_ADC_3[10] = {0};

// Command constants for starting conversion
const uint8_t CMD_START_CONVERSION = 0x20;
static int wait_time = 120;  // Delay between operations

// Result and register variables for ADC operations
uint8_t res;
uint8_t read_register = 0;
uint8_t write_reagister = 0;

// Variables for storing temperature sensor readings
float temp_sensor[3][4] = {0};

// Variables for storing temperature sensor readings
uint16_t sensor_hex[3][4] = {0};
/***********************************************************************************
 * Callendar-Van Dusen Equation for a More Accurate Conversion
For a more accurate range (e.g., -200°C to 850°C), the Callendar-Van Dusen equation is:

Above 0°C

	Rt = R0[1 + At + Bt2 + C(t - 100)t3]
 */

const float R0 = 100.0;  // Resistance at 0°C
const float A = 3.9083e-3;
const float B = -5.775e-7;
float temperature;

uint16_t update_sensor_table = 7000;
uint8_t check_channel = 0 ;


// Function to handle ADC operations
uint8_t ADC_operation() {
	float sensor_value;

	// Loop through each ADC and channel
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			sensor_value = ADC_READ(i, j);  // Read ADC value
			if (sensor_value > 1500 || sensor_value < -1000) {  // Check for out-of-range values
				sprintf(buffer, "S%derrorE", j);  // Store error message in buffer
			}
			HAL_Delay(1);  // Delay between readings
		}


	}
	//HAL_GPIO_TogglePin(GPIOE, LED1_Pin);  // Toggle status LED
}

uint8_t ADC_config(uint8_t ADC_num)
{
	uint8_t result=0;
	txd_buf_1[0] = 0x40;
	txd_buf_1[1] = 0x11;
	txd_buf_1[2] = 0x0c; // WREG write AIN10-AINCOM,(1010 1100) binary,MUXP and MUXN selection
	txd_buf_1[3] = 0x80; // gain =1
	txd_buf_1[4] = 0x76; // DATA RATE 20 SPS
	txd_buf_1[5] = 0xEA; //REF on (0001 10-ref 01)
	txd_buf_1[6] = 0x1E; // IDAC MAG
	txd_buf_1[7] = 0x02; // IDAC MUX
	txd_buf_1[8] = 0x07; // Vbias
	txd_buf_1[9] = 0xfb; // Timeout bit second =1 ,default is 10h
	txd_buf_1[10] = 0x00;  //
	txd_buf_1[11] = 0x18;  //
	txd_buf_1[12] = 0x0;  //
	txd_buf_1[13] = 0x0;  //
	txd_buf_1[14] = 0x0;  //
	txd_buf_1[15] = 0x0;  //
	txd_buf_1[16] = 0x0;  //
	txd_buf_1[17] = 0x40;  //
	txd_buf_1[18] = 0x00;  //
	txd_buf_1[19] = 0x00;  //
	txd_buf_1[20] = 0x00;  //
	txd_buf_1[18] = 0x00;  //



	txd_buf_2[0] = 0x40;
	txd_buf_2[1] = 0x11;
	txd_buf_2[2] = 0x0c; // WREG write AIN10-AINCOM,(1010 1100) binary,MUXP and MUXN selection
	txd_buf_2[3] = 0x80; // gain =1
	txd_buf_2[4] = 0x76; // DATA RATE 20 SPS
	txd_buf_2[5] = 0xEA; //REF on (0001 10-ref 01)
	txd_buf_2[6] = 0x1E; // IDAC MAG
	txd_buf_2[7] = 0x02; // IDAC MUX
	txd_buf_2[8] = 0x07; // Vbias
	txd_buf_2[9] = 0xfb; // Timeout bit second =1 ,default is 10h
	txd_buf_2[10] = 0x00;  //
	txd_buf_2[11] = 0x18;  //
	txd_buf_2[12] = 0x0;  //
	txd_buf_2[13] = 0x0;  //
	txd_buf_2[14] = 0x0;  //
	txd_buf_2[15] = 0x0;  //
	txd_buf_2[16] = 0x0;  //
	txd_buf_2[17] = 0x40;  //
	txd_buf_2[18] = 0x00;  //
	txd_buf_2[19] = 0x00;  //
	txd_buf_2[20] = 0x00;  //
	txd_buf_2[18] = 0x00;  //



	txd_buf_3[0] = 0x40;
	txd_buf_3[1] = 0x11;
	txd_buf_3[2] = 0x0c; // WREG write AIN10-AINCOM,(1010 1100) binary,MUXP and MUXN selection
	txd_buf_3[3] = 0x80; // gain =1
	txd_buf_3[4] = 0x76; // DATA RATE 20 SPS
	txd_buf_3[5] = 0xEA; //REF on (0001 10-ref 01)
	txd_buf_3[6] = 0x1E; // IDAC MAG
	txd_buf_3[7] = 0x02; // IDAC MUX
	txd_buf_3[8] = 0x07; // Vbias
	txd_buf_3[9] = 0xfb; // Timeout bit second =1 ,default is 10h
	txd_buf_3[10] = 0x00;  //
	txd_buf_3[11] = 0x18;  //
	txd_buf_3[12] = 0x0;  //
	txd_buf_3[13] = 0x0;  //
	txd_buf_3[14] = 0x0;  //
	txd_buf_3[15] = 0x0;  //
	txd_buf_3[16] = 0x0;  //
	txd_buf_3[17] = 0x40;  //
	txd_buf_3[18] = 0x00;  //
	txd_buf_3[19] = 0x00;  //
	txd_buf_3[20] = 0x00;  //
	txd_buf_3[18] = 0x00;  //

	switch( ADC_num)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOC, RESET_1_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOC, RESET_1_Pin, 0);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOC, RESET_1_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOF, START_1_Pin, 0);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);


		HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf_1,0x13,  0xFFFFFFFF);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		txd_buf_1[0] = 0x20;
		txd_buf_1[1] = 0x11;

		if(HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf_1, 2,  0xFFFFFFFF) != HAL_OK)
		{
			HAL_Delay(1);// Handle transmission error
		}
		HAL_Delay(1);
		if(HAL_SPI_Receive(&hspi5, (uint8_t*) &rxd_buf_1, 0x11,  0xFFFFFFFF) != HAL_OK)
		{
			HAL_Delay(1);// Handle transmission error
		}


		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

		if(rxd_buf_1[7]==txd_buf_1[9] )
		{
			result = 1;

		}

		break;
	case 2:
		HAL_GPIO_WritePin(GPIOG, RESET_2_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOG, RESET_2_Pin, 0);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOG, RESET_2_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOG, START_2_Pin, 0);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		HAL_Delay(1);

		HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf_2,0x13,  0xFFFFFFFF);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);

		txd_buf_2[0] = 0x20;
		txd_buf_2[1] = 0x11;



		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		if(HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf_2, 2,  0xFFFFFFFF) != HAL_OK) {
			HAL_Delay(1);// Handle transmission error
		}
		HAL_Delay(1);
		if(HAL_SPI_Receive(&hspi2, (uint8_t*) &rxd_buf_2, 0x11,  0xFFFFFFFF) != HAL_OK) {
			HAL_Delay(1);// Handle transmission error
		}
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOG, START_2_Pin, 1);
		HAL_Delay(1);
		if(rxd_buf_2[7]==txd_buf_2[9] )
		{
			result = 1;

		}

		break;
	case 3:

		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 0);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 1);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, START_3_Pin, 0);

		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		HAL_Delay(1);

		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf_3,0x13, 0xFFFFFFFF);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
		HAL_Delay(1);

		txd_buf_3[0] = 0x20;
		txd_buf_3[1] = 0x11;

		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		HAL_Delay(1);
		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf_3, 2,  0xFFFFFFFF);
		HAL_Delay(1);
		HAL_SPI_Receive(&hspi3, (uint8_t*) &rxd_buf_3, 0x11,  0xFFFFFFFF);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOA, START_3_Pin, 1);
		HAL_Delay(1);

		if(rxd_buf_3[7]==txd_buf_3[9] )
		{
			result = 1;

		}
		break;
	}

	return result;


}

#if 1

uint16_t ADC_READ(uint8_t adc_num,uint8_t channel)
{
	//	uint8_t txd_buf[30]={0};
	uint8_t rxd_buf[30]={0};
	uint8_t status;
	uint8_t wirestatus=0;

	uint16_t sensor_value = 0x7FFF;
	float resistance = 0;
	uint32_t sum = 0;           // Sum for averaging
	uint16_t adchex=0;
	float average = 0.0;        // Average value


	txd_buf[0] = 0x40; // To write data from 0 register
	txd_buf[1] = 0x11;	//	Number of register to write
	txd_buf[2] = 0x0C; // Device ID
	txd_buf[3] = 0x00; // Status of Device it is read only
	txd_buf[4] = 0x76; // Input MUX for RTD legs selection (MUX configuration)
	txd_buf[5] = 0xEA; //PGA Gain = 1
	txd_buf[6] = 0x1E; // Data rate = 4000 SPS
	txd_buf[7] = 0x82; // Reference configuration 02  change this
	txd_buf[8] = 0x87; // IDAC magnitude
	txd_buf[9] = 0xfb; // IDAC MUX
	txd_buf[10] = 0x00; // Vbias
	txd_buf[11] = 0x10; // Timeout configuration
	txd_buf[12] = 0x0;  //
	txd_buf[13] = 0x0;  //
	txd_buf[14] = 0x0;  //
	txd_buf[15] = 0x0;  //
	txd_buf[16] = 0x0;  //
	txd_buf[17] = 0x40;  //
	txd_buf[18] = 0x00;  //
	txd_buf[19] = 0x00;  //
	txd_buf[20] = 0x00;  //
	txd_buf[18] = 0x00;  //


	//
	//	txd_buf[0] = 0x40;
	//	txd_buf[1] = 0x11;
	//	txd_buf[2] = 0x0c; //  	ADS114S08 (12 channels, 16 bits)
	//	txd_buf[3] = 0x80; //  	POR event occurred and has not been cleared. Flag must be cleared by user register write (default).
	//	txd_buf[4] = 0x76; // 	DATA RATE 20 SPS
	//	txd_buf[5] = 0xEA; //	REF on (0001 10-ref 01)0x4a
	//	txd_buf[6] = 0x1E; //	4000SPS
	//	txd_buf[7] = 0x02;
	//	txd_buf[8] = 0x07;
	//	txd_buf[9] = 0xfb;
	//	txd_buf[10] = 0x00;
	//	txd_buf[11] = 0x10;
	//	txd_buf[12] = 0x0;
	//	txd_buf[13] = 0x0;
	//	txd_buf[14] = 0x0;
	//	txd_buf[15] = 0x0;
	//	txd_buf[16] = 0x0;
	//	txd_buf[17] = 0x40;
	//	txd_buf[18] = 0x00;
	//	txd_buf[19] = 0x00;
	//	txd_buf[20] = 0x00;
	//	txd_buf[18] = 0x00;




	switch(channel)
	{
	case 0:
		txd_buf[4] = 0x43;// Input MUX for RTD legs selection (MUX configuration)
		txd_buf[9] = 0xf5; // IDAC MUX
		break;

	case 1:
		txd_buf[4] = 0x10; // Input MUX for RTD legs selection (MUX configuration)
		txd_buf[9] = 0xf2; // IDAC MUX
		break;

	case 2:
		txd_buf[4] = 0x9A; // Input MUX for RTD legs selection (MUX configuration)
		txd_buf[9] = 0xf8; // IDAC MUX
		break;
	case 3:
		txd_buf[4] = 0x76; // Input MUX for RTD legs selection (MUX configuration)
		txd_buf[9] = 0xfb;// IDAC MUX
		break;
	}

	switch(adc_num)
	{
	case ADC_1:
		//		if(sensor_table[adc_num][channel] == 1 || check_channel)
		//		{
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		HAL_GPIO_WritePin(GPIOF, START_1_Pin, 0);

		//HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOC, RESET_1_Pin, 0);
		//HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOC, RESET_1_Pin, 1);
		//HAL_Delay(100);



		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);


		HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf,0x13, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);


		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOF, START_1_Pin, 1);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		txd_buf[0] = 0x19;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf, 1, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

		HAL_Delay(wait_time);


		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		txd_buf[0] = 0x13;
		//HAL_Delay(2);
		for (int i = 0; i < 10; i++)
		{

			HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf, 1, 0xFF);
			//HAL_Delay(2);
			HAL_SPI_Receive(&hspi5, (uint8_t*) &reg_data_ADC_1[i], 0x2, 0xFF);


			// Byte-wise flip
			reg_data_ADC_1[i] = (reg_data_ADC_1[i] >> 8) | (reg_data_ADC_1[i] << 8);
			sum += reg_data_ADC_1[i];  // Add to sum for averaging

		}

		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

		average = sum / 10.0;
		adchex  = sum / 10;
		resistance = ((2*1000*(float)average)/4) / 0xFFFF;
		//Callendar-Van Dusen Equation
		if (resistance >= R0) {  // Above 0°C
			temperature = (-A + sqrt(A * A - 4 * B * (1 - (resistance / R0)))) / (2 * B);
		}
		else
		{  // Below 0°C, use simpler approximation
			temperature = (resistance - R0) / 0.385;
		}

		if(rxd_buf[7]==txd_buf[9])
		{
			HAL_GPIO_WritePin(GPIOE, LED1_Pin, 1); // Toggle LED for visual feedback
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOE, LED1_Pin, 0);
			//HAL_Delay(1);
			//			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_1[1],temperature,resistance);
			//			HAL_UART_Transmit(&huart4, buffer, sizeof(buffer),0xfffff);
#ifdef ADC_UART
			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_1[1],temperature,resistance);
			HAL_UART_Transmit_IT(&huart5, buffer, strlen((char *)(buffer)));
			HAL_Delay(20);
#endif

		}
		//}
#if 1

		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		txd_buf[0] = 0x21;
		txd_buf[1] = 0x00;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf, 2, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi5, (uint8_t*) &wirestatus, 0x01, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

		//		status = wire_break(wirestatus);

		if (wire_break(wirestatus))
		{
			status=0;
			wirestatus=0;
			temperature = 0;
			adchex =0;
			memset(&reg_data_ADC_1,0,sizeof(reg_data_ADC_1));
		}
		else if(check_sensor_validity(adchex))
		{
			wirestatus=0;
			temperature = 0;
			adchex =0;
			memset(&reg_data_ADC_1,0,sizeof(reg_data_ADC_1));
		}
		wirestatus=0;
#endif
		temp_sensor[adc_num][channel] = temperature ;
		sensor_hex[adc_num][channel] = 	adchex ;

		sensor_value =	sum / 10;

		break;

	case ADC_2:
		//		if(sensor_table[adc_num][channel] == 1 || check_channel)
		//		{
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		HAL_GPIO_WritePin(GPIOG, START_2_Pin, 0);
		//HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOG, RESET_2_Pin, 0);
		//HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOG, RESET_2_Pin, 1);
		//HAL_Delay(100);



		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);

		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf,0x13, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);


		//HAL_Delay(200);

		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOG, START_2_Pin, 1);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		txd_buf[0] = 0x19;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf, 1, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);
		HAL_Delay(2);
		HAL_Delay(wait_time);



		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		txd_buf[0] = 0x13;
		HAL_Delay(2);
		for (int i = 0; i < 10; i++)
		{

			HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf, 1, 0xFF);
			HAL_Delay(2);
			HAL_SPI_Receive(&hspi2, (uint8_t*) &reg_data_ADC_2[i], 0x2, 0xFF);

			// Byte-wise flip
			reg_data_ADC_2[i] = (reg_data_ADC_2[i] >> 8) | (reg_data_ADC_2[i] << 8);
			sum += reg_data_ADC_2[i];  // Add to sum for averaging
			//			if(check_sensor_validity( reg_data_ADC_2[i] ))
			//			{
			//				sensor_table[adc_num][channel] = 0 ;
			//				temp_sensor[adc_num][channel] = 0 ;
			//				sensor_hex[adc_num][channel] = 0 ;
			//				return 0x7FFF;
			//			}
		}
		adchex =  sum / 10;
		average = sum / 10.0;
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);
		resistance = ((2*1000*(float)average)/4) / 0xFFFF;
		//Callendar-Van Dusen Equation
		if (resistance >= R0) {  // Above 0°C
			temperature = (-A + sqrt(A * A - 4 * B * (1 - (resistance / R0)))) / (2 * B);
		}
		else
		{  // Below 0°C, use simpler approximation
			temperature = (resistance - R0) / 0.385;
		}
		if(rxd_buf[7]==txd_buf[9] )
		{
			HAL_GPIO_WritePin(GPIOE, LED2_Pin, 1); // Toggle LED for visual feedback
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOE, LED2_Pin, 0);
			HAL_Delay(1);
			//			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_2[1],temperature,resistance);
			//			HAL_UART_Transmit(&huart4, buffer, sizeof(buffer),0xfffff);

#ifdef ADC_UART
			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_2[1],temperature,resistance);
			HAL_UART_Transmit_IT(&huart5, buffer, strlen((char *)(buffer)));
			HAL_Delay(20);
#endif

		}

#if 1
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		txd_buf[0] = 0x21;
		txd_buf[1] = 0x00;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf, 2, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi2, (uint8_t*) &wirestatus, 0x01, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);

		//		status = wire_break(wirestatus);


		if (wire_break(wirestatus))
		{
			status=0;
			wirestatus=0;
			temperature = 0;
			adchex=0;
			memset(&reg_data_ADC_2,0,sizeof(reg_data_ADC_2));
		}
		else if(check_sensor_validity(adchex))
		{
			wirestatus=0;
			temperature = 0;
			adchex =0;
			memset(&reg_data_ADC_2,0,sizeof(reg_data_ADC_2));
		}
		wirestatus=0;
#endif
		temp_sensor[adc_num][channel] = temperature ;
		sensor_hex[adc_num][channel] = adchex ;

		sensor_value = sum / 10;

		break;

	case ADC_3:
		//		if(sensor_table[adc_num][channel] == 1 || check_channel)
		//		{
		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 0);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 1);
		//HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		HAL_GPIO_WritePin(GPIOA, START_3_Pin, 0);


		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);


		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf,0x13, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);


		//HAL_Delay(200);


		HAL_GPIO_WritePin(GPIOA, START_3_Pin, 1);

		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		txd_buf[0] = 0x19;

		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf, 1, 0xFF);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
		HAL_Delay(wait_time);

		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		txd_buf[0] = 0x13;
		for (int i = 0; i < 10; i++)
		{
			HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf, 1, 0xFFFFFF);

			HAL_SPI_Receive(&hspi3, (uint8_t*) &reg_data_ADC_3[i], 0x2, HAL_MAX_DELAY);


			// Byte-wise flip
			reg_data_ADC_3[i] = (reg_data_ADC_3[i] >> 8) | (reg_data_ADC_3[i] << 8);
			sum += reg_data_ADC_3[i];  // Add to sum for averaging
			//				if(check_sensor_validity( reg_data_ADC_3[i] ))
			//				{
			//					sensor_table[adc_num][channel] = 0 ;
			//					temp_sensor[adc_num][channel] = 0 ;
			//					sensor_hex[adc_num][channel] = 0 ;
			//					return 0x7FFF;
		}

		adchex = sum / 10;
		average = sum / 10.0;
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
		resistance = ((2*1000*(float)average)/4) / 0xFFFF;


		//Callendar-Van Dusen Equation
		if (resistance >= R0) {  // Above 0°C
			temperature = (-A + sqrt(A * A - 4 * B * (1 - (resistance / R0)))) / (2 * B);
		}
		else
		{  // Below 0°C, use simpler approximation
			temperature = (resistance - R0) / 0.385;
		}
		if(rxd_buf[7]==txd_buf[9])
		{
			HAL_GPIO_WritePin(GPIOE, LED3_Pin, 1); // Toggle LED for visual feedback
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOE, LED3_Pin, 0);
			//			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_3[1],temperature,resistance);
			//			HAL_UART_Transmit(&huart4, buffer, sizeof(buffer),0xfffff);

#ifdef ADC_UART
			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_3[1],temperature,resistance);
			HAL_UART_Transmit_IT(&huart5, buffer, strlen((char *)(buffer)));
			HAL_Delay(20);
#endif

		}

#if 1
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		txd_buf[0] = 0x21;
		txd_buf[1] = 0x00;
		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf, 2, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi3, (uint8_t*) &wirestatus, 0x01, 0xFF);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);

		//		status = wire_break(wirestatus);

		if (wire_break(wirestatus))
		{
			status=0;
			wirestatus=0;
			temperature = 0;
			adchex=0;
			memset(&reg_data_ADC_3,0,sizeof(reg_data_ADC_3));
		}
		else if(check_sensor_validity(adchex))
		{
			wirestatus=0;
			temperature = 0;
			adchex =0;
			memset(&reg_data_ADC_3,0,sizeof(reg_data_ADC_3));
		}
		wirestatus=0;
#endif
		temp_sensor[adc_num][channel] = temperature ;
		sensor_hex[adc_num][channel] = adchex ;

		sensor_value = sum / 10;

		break;
	}


	return sensor_value;

}
#endif



void get_sensor_status()
{
	uint16_t sensor_value = 0;
	check_channel = 1;
	// Loop through each ADC and channel
	for (uint8_t adc = 0; adc < 3; adc++) {
		for (uint8_t channel = 0; channel < 4; channel++) {
			if(sensor_table[adc][channel] == 0 )
			{
				sensor_value = ADC_READ(adc, channel);  // Read ADC value

				if(!check_sensor_validity(sensor_value))
				{
					sensor_table[adc][channel] = 1 ;
				}
				else
				{
					sensor_table[adc][channel] = 0 ;
				}
				// sprintf(buffer, "S  %d  %dE  %X\n\r",adc,channel,sensor_value);
				// HAL_UART_Transmit(&huart5, buffer, strlen((char *)(buffer)),0xFFFFFF0);
				// HAL_Delay(20);
			}

		}
		HAL_Delay(10);  // Delay between readings
	}

	check_channel = 0 ;
}


uint8_t check_sensor_validity(uint16_t data)
{
	int i=0;
	if(data<ADC_MIN_VAL || data>ADC_MAX_VAL)
		return 1 ;
	//		for(i=0; i<sizeof(Error_table); i++)
	//		{
	//			if(data==Error_table[i])
	//			{
	//				return 1 ;
	//			}
	//		}
	return 0 ;

}

uint8_t wire_break(uint8_t flag)
{
	uint8_t FL_REF =0;

	if(txd_buf[7] == 0xC2)//0xC2
	{
		FL_REF= 1;
	}
	else if(txd_buf[7] == 0x82)//0x82
	{
		FL_REF= 2;
	}
	else
	{
		FL_REF=0;
	}

	switch(FL_REF)
	{
	case 1:
		if(0x01 == flag)  //0x29 w/o interface card :  0x01  with interface card
			return 1;

		else if(0x00 == flag) //0x00 w/o interface card :  0x00  with interface card
			return 0;

		break;
	case 2:
		if(0x2b == flag) //0x2b w/o interface card :  0x03  with interface card
			return 1;

		else if(0x2 == flag) //0x2 w/o interface card :  0x02  with interface card
			return 0;

		break;
	default:
		return 0;

		break;
	}
	return 0;
}
