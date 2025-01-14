/*
 * ADC_Operations.c
 *
 *  Created on: Sep 9, 2024
 *      Author: VIMAL
 */
/*
 * time_setter.c
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

float ADC_READ(uint8_t adc_num,uint8_t channel);
/* USER CODE END Includes */
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern SPI_HandleTypeDef hspi5;


extern SRAM_HandleTypeDef hsram1;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart10;

extern uint8_t ATP_test;

const uint8_t ADC_ID = 0x00; //00000011;
const uint8_t ADC_STATUS = 0x01; //00000011;
const uint8_t ADC_INPMUX = 0x02; //00000011;
const uint8_t ADC_PGA = 0x03; //00000011;
const uint8_t ADC_DATARATE = 0x04; //00000011;
const uint8_t ADC_REF = 0x05; //00000011;
const uint8_t ADC_IDACMAG = 0x06; //00000011;
const uint8_t ADC_IDACMUX = 0x07; //00000011;
const uint8_t ADC_VBIAS = 0x08; //00000011;
const uint8_t ADC_SYS = 0x09; //00000011;
const uint8_t ADC_RESERVED_0 = 0x0A; //00000011;
const uint8_t ADC_OFCAL0 = 0x0B; //00000011;
const uint8_t ADC_OFCAL1 = 0x0C; //00000011;
const uint8_t ADC_RESERVED_1 = 0x0D; //00000011;
const uint8_t ADC_FSCAL0 = 0x0E; //00000011;
const uint8_t ADC_FSCAL1 = 0x0F; //00000011;
const uint8_t ADC_GPIODAT = 0x10; //00000011;
const uint8_t ADC_GPIOCON = 0x11; //00000011;

// COmmands
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

uint8_t buffer[300]={0};
uint8_t buffer_1[300]={0};
uint32_t count=0;

//GPIO_TypeDef *GPIO;
static int wait_time = 50;
uint8_t txd_buf_1[30] = { 0 };
uint8_t rxd_buf_1[30] = { 0 };
uint8_t txd_buf_2[30] = { 0 };
uint8_t rxd_buf_2[30] = { 0 };
uint8_t txd_buf_3[30] = { 0 };
uint8_t rxd_buf_3[30] = { 0 };
uint16_t reg_data_ADC_1=0;//[4]= {0};
uint16_t reg_data_ADC_2=0;//[4]= {0};
uint16_t reg_data_ADC_3=0;//[4]= {0};
uint8_t res;
uint8_t read_register = 0;
uint8_t write_reagister = 0;


float resistance ;
float temperature ;

float temp_sensor_1=0;
float temp_sensor_2=0;
float temp_sensor_3=0;
float temp_sensor_4=0;
float temp_sensor_5=0;
float temp_sensor_6=0;
float temp_sensor_7=0;
float temp_sensor_8=0;
float temp_sensor_9=0;
float temp_sensor_10=0;
float temp_sensor_11=0;
float temp_sensor_12=0;



uint8_t ADC_operation()
{

	float sensor_value;

	for(uint8_t i = 1 ;i<=3;i++)
	{
		for(uint8_t j = 1 ;j<=4;j++)
		{
			sensor_value =	ADC_READ(i,j);
			if(sensor_value > 1500 | sensor_value < -1000)
			{
				sprintf(buffer, "S%derrorE",j);
			}
			else
			{
				//sprintf(buffer, "S  %d  %.2fE",count++, sensor_value);
			}


			HAL_Delay(1);

			//HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer), 0xFFFF);
			//HAL_Delay(10);

		}
	}
	HAL_GPIO_TogglePin(GPIOE, LED1_Pin);


}

#if 0

float ADC_READ(uint8_t adc_num,uint8_t channel)
{


	/*
	txd_buf[0] = 0x40;
	txd_buf[1] = 0x11;
	txd_buf[2] = 0x0c; // WREG write AIN10-AINCOM,(1010 1100) binary,MUXP and MUXN selection
	txd_buf[3] = 0x80; // gain =1
	txd_buf[4] = 0x76; // DATA RATE 20 SPS
	txd_buf[5] = 0xEA; //REF on (0001 10-ref 01)
	txd_buf[6] = 0x1E; // IDAC MAG
	txd_buf[7] = 0x02; // IDAC MUX
	txd_buf[8] = 0x07; // Vbias
	txd_buf[9] = 0xfb; // Timeout bit second =1 ,default is 10h
	txd_buf[10] = 0x00; // start converting command
	txd_buf[11] = 0x10; // start converting command
	txd_buf[12] = 0x0; // start converting command
	txd_buf[13] = 0x0; // start converting command
	txd_buf[14] = 0x0; // start converting command
	txd_buf[15] = 0x0; // start converting command
	txd_buf[16] = 0x0; // start converting command
	txd_buf[17] = 0x40; // start converting command
	txd_buf[18] = 0x00; // start converting command
	txd_buf[19] = 0x00; // start converting command
	txd_buf[20] = 0x00; // start converting command
	txd_buf[18] = 0x00; // start converting command
	 */


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
	txd_buf_1[10] = 0x00; // start converting command
	txd_buf_1[11] = 0x18; // start converting command
	txd_buf_1[12] = 0x0; // start converting command
	txd_buf_1[13] = 0x0; // start converting command
	txd_buf_1[14] = 0x0; // start converting command
	txd_buf_1[15] = 0x0; // start converting command
	txd_buf_1[16] = 0x0; // start converting command
	txd_buf_1[17] = 0x40; // start converting command
	txd_buf_1[18] = 0x00; // start converting command
	txd_buf_1[19] = 0x00; // start converting command
	txd_buf_1[20] = 0x00; // start converting command
	txd_buf_1[18] = 0x00; // start converting command



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
	txd_buf_2[10] = 0x00; // start converting command
	txd_buf_2[11] = 0x18; // start converting command
	txd_buf_2[12] = 0x0; // start converting command
	txd_buf_2[13] = 0x0; // start converting command
	txd_buf_2[14] = 0x0; // start converting command
	txd_buf_2[15] = 0x0; // start converting command
	txd_buf_2[16] = 0x0; // start converting command
	txd_buf_2[17] = 0x40; // start converting command
	txd_buf_2[18] = 0x00; // start converting command
	txd_buf_2[19] = 0x00; // start converting command
	txd_buf_2[20] = 0x00; // start converting command
	txd_buf_2[18] = 0x00; // start converting command



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
	txd_buf_3[10] = 0x00; // start converting command
	txd_buf_3[11] = 0x18; // start converting command
	txd_buf_3[12] = 0x0; // start converting command
	txd_buf_3[13] = 0x0; // start converting command
	txd_buf_3[14] = 0x0; // start converting command
	txd_buf_3[15] = 0x0; // start converting command
	txd_buf_3[16] = 0x0; // start converting command
	txd_buf_3[17] = 0x40; // start converting command
	txd_buf_3[18] = 0x00; // start converting command
	txd_buf_3[19] = 0x00; // start converting command
	txd_buf_3[20] = 0x00; // start converting command
	txd_buf_3[18] = 0x00; // start converting command



	switch(channel)
	{
	case 1:
		txd_buf_1[4] = 0x43; // DATA RATE 20 SPS
		txd_buf_1[7] = 0x02; // IDAC MUX
		txd_buf_1[9] = 0xf5; // Timeout bit second =1 ,default is 10h


		txd_buf_2[4] = 0x43; // DATA RATE 20 SPS
		txd_buf_2[7] = 0x02; // IDAC MUX
		txd_buf_2[9] = 0xf5; // Timeout bit second =1 ,default is 10h

		txd_buf_3[4] = 0x43; // DATA RATE 20 SPS
		txd_buf_3[7] = 0x02; // IDAC MUX
		txd_buf_3[9] = 0xf5; // Timeout bit second =1 ,default is 10h
		break;

	case 2:
		txd_buf_1[4] = 0x10; // DATA RATE 20 SPS
		txd_buf_1[7] = 0x02; // IDAC MUX
		txd_buf_1[9] = 0xf2; // Timeout bit second =1 ,default is 10h

		txd_buf_2[4] = 0x10; // DATA RATE 20 SPS
		txd_buf_2[7] = 0x02; // IDAC MUX
		txd_buf_2[9] = 0xf2; // Timeout bit second =1 ,default is 10h
		txd_buf_3[4] = 0x10; // DATA RATE 20 SPS
		txd_buf_3[7] = 0x02; // IDAC MUX
		txd_buf_3[9] = 0xf2; // Timeout bit second =1 ,default is 10h
		break;

	case 3:
		txd_buf_1[4] = 0x9A; // DATA RATE 20 SPS
		txd_buf_1[7] = 0x02; // IDAC MUX
		txd_buf_1[9] = 0xf8; // Timeout bit second =1 ,default is 10h
		txd_buf_2[4] = 0x9A; // DATA RATE 20 SPS
		txd_buf_2[7] = 0x02; // IDAC MUX
		txd_buf_2[9] = 0xf8; // Timeout bit second =1 ,default is 10h
		txd_buf_3[4] = 0x9A; // DATA RATE 20 SPS
		txd_buf_3[7] = 0x02; // IDAC MUX
		txd_buf_3[9] = 0xf8; // Timeout bit second =1 ,default is 10h
		break;
	case 4:
		txd_buf_1[4] = 0x76; // DATA RATE 20 SPS
		txd_buf_1[7] = 0x02; // IDAC MUX
		txd_buf_1[9] = 0xfb; // Timeout bit second =1 ,default is 10h
		txd_buf_2[4] = 0x76; // DATA RATE 20 SPS
		txd_buf_2[7] = 0x02; // IDAC MUX
		txd_buf_2[9] = 0xfb; // Timeout bit second =1 ,default is 10h
		txd_buf_3[4] = 0x76; // DATA RATE 20 SPS
		txd_buf_3[7] = 0x02; // IDAC MUX
		txd_buf_3[9] = 0xfb; // Timeout bit second =1 ,default is 10h
		break;
	}

	HAL_GPIO_WritePin(GPIOF, START_1_Pin, 0);
	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);


	HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf_1,0x13,  0xFFFFFFFF);
	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
	txd_buf_1[0] = 0x20;
	txd_buf_1[1] = 0x11;

	if(HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf_1, 2,  0xFFFFFFFF) != HAL_OK) {
		HAL_Delay(1);// Handle transmission error
	}
	HAL_Delay(1);
	if(HAL_SPI_Receive(&hspi5, (uint8_t*) &rxd_buf_1, 0x11,  0xFFFFFFFF) != HAL_OK) {
		HAL_Delay(1);// Handle transmission error
	}


	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);
	HAL_GPIO_WritePin(GPIOF, START_1_Pin, 1);


	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
	//txd_buf_1[0] = 0x19;

	HAL_SPI_Transmit(&hspi5, (uint8_t*) &CMD_SFOCAL, 1,  0xFFFFFFFF);
	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);






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
	//txd_buf_2[0] = 0x19;


	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi2, (uint8_t*) &CMD_SFOCAL, 1,  0xFFFFFFFF);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);
	HAL_Delay(1);


	HAL_Delay(1);

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

	//txd_buf_3[0] = 0x19;
	HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &CMD_SFOCAL, 1,  0xFFFFFFFF);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
	HAL_Delay(1);



	HAL_Delay(wait_time);

	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
	//txd_buf_1[0] = 0x13;

	HAL_SPI_Transmit(&hspi5, (uint8_t*) &CMD_RDATA, 1,  0xFFFFFFFF);
	HAL_SPI_Receive(&hspi5, (uint8_t*) &reg_data_ADC_1, 0x4,  0xFFFFFFFF);
	HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);



	//txd_buf_2[0] = 0x13;

	HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi2, (uint8_t*) &CMD_RDATA, 1,  0xFFFFFFFF);
	HAL_Delay(1);
	HAL_SPI_Receive(&hspi2, (uint8_t*) &reg_data_ADC_2, 0x4,  0xFFFFFFFF);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);

	//txd_buf_3[0] = 0x13;
	HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
	HAL_Delay(1);
	HAL_SPI_Transmit(&hspi3, (uint8_t*) &CMD_RDATA, 1,  0xFFFFFFFF);
	HAL_Delay(1);
	HAL_SPI_Receive(&hspi3, (uint8_t*) &reg_data_ADC_3, 0x4,  0xFFFFFFFF);
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
	HAL_Delay(1);
	memset(buffer,0,sizeof(buffer));
	if(rxd_buf_1[7]==txd_buf_1[9] )
	{

		sprintf(buffer, "S 111 %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_1,temperature,resistance);
		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		HAL_Delay(10);
	}
	memset(buffer,0,sizeof(buffer));
	if(rxd_buf_2[7]==txd_buf_2[9] )
	{

		sprintf(buffer, "S 222 %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_2,temperature,resistance);
		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		HAL_Delay(10);
	}
	memset(buffer,0,sizeof(buffer));
	if(rxd_buf_3[7]==txd_buf_3[9] )
	{
		sprintf(buffer, "S 333 %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_3,temperature,resistance);
		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		HAL_Delay(10);
	}


	reg_data_ADC_1 =reg_data_ADC_1 & 0xffff;
	reg_data_ADC_1 = (reg_data_ADC_1 >> 8 | reg_data_ADC_1 << 8 ) ;
	reg_data_ADC_1 =reg_data_ADC_1 & 0xffff;
	reg_data_ADC_2 =reg_data_ADC_2 & 0xffff;
	reg_data_ADC_2 = (reg_data_ADC_2 >> 8 | reg_data_ADC_2 << 8 ) ;
	reg_data_ADC_2 =reg_data_ADC_2 & 0xffff;
	reg_data_ADC_3 =reg_data_ADC_3 & 0xffff;
	reg_data_ADC_3 = (reg_data_ADC_3 >> 8 | reg_data_ADC_3 << 8 ) ;
	reg_data_ADC_3 =reg_data_ADC_3 & 0xffff;

	if(channel == 1)
	{
		resistance = ((2*1000*reg_data_ADC_1)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;

		temp_sensor_1 = temperature;
		resistance = ((2*1000*reg_data_ADC_2)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_2 = temperature;
		resistance = ((2*1000*reg_data_ADC_3)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_3 =temperature;
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_1", temp_sensor_1);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_2", temp_sensor_2);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_3", temp_sensor_3);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
	}
	else if(channel == 2)
	{
		resistance = ((2*1000*reg_data_ADC_1)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;

		temp_sensor_4 = temperature;
		resistance = ((2*1000*reg_data_ADC_2)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_5 = temperature;
		resistance = ((2*1000*reg_data_ADC_3)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_6 =temperature;


		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_4", temp_sensor_4);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_5", temp_sensor_5);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_6", temp_sensor_6);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
	}
	else if(channel == 3)
	{
		resistance = ((2*1000*reg_data_ADC_1)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;

		temp_sensor_7 = temperature;
		resistance = ((2*1000*reg_data_ADC_2)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_8 = temperature;
		resistance = ((2*1000*reg_data_ADC_3)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_9 =temperature;
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_7", temp_sensor_7);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_8", temp_sensor_8);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_9", temp_sensor_9);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
	}
	else if(channel == 4)
	{
		resistance = ((2*1000*reg_data_ADC_1)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;

		temp_sensor_10 = temperature;
		resistance = ((2*1000*reg_data_ADC_2)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_11 = temperature;
		resistance = ((2*1000*reg_data_ADC_3)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		temp_sensor_12 =temperature;
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_10", temp_sensor_10);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_11", temp_sensor_11);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		sprintf(buffer, "S  %s  %.2fE\n\r","temp_sensor_12", temp_sensor_12);
		//		HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
	}


	resistance = ((2*1000*reg_data_ADC_1)/4) / 0xFFFF;
	temperature = (resistance - 100.0 ) / 0.385;

	if(channel == 10)
	{
		uint8_t time_data[8]={0};
		uint32_t sec_addr = 0x60000000 | 0x7FFF9;
		HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&time_data,7);
		memset(buffer,0,sizeof(buffer));

		sprintf(buffer,
				"{\"TS1\": \"%.2f\", \"TS2\": \"%.2f\", \"TS3\": \"%.2f\", \"TS4\": \"%.2f\", \"TS5\": \"%.2f\", "
				"\"TS6\": \"%.2f\", \"TS7\": \"%.2f\", \"TS8\": \"%.2f\", \"TS9\": \"%.2f\", \"TS10\": \"%.2f\", "
				"\"TS11\": \"%.2f\", \"TS12\": \"%.2f\", \"TIME\": \"%x:%x:%x\",\"DATE\": \"%x-%x-20%x}\"\n\r",
				temp_sensor_1, temp_sensor_2, temp_sensor_3, temp_sensor_4, temp_sensor_5, temp_sensor_6,
				temp_sensor_7, temp_sensor_8, temp_sensor_9, temp_sensor_10, temp_sensor_11, temp_sensor_12,time_data[2],time_data[1],time_data[0],time_data[4],time_data[5],time_data[6]);
		//HAL_UART_Transmit_IT(&huart8, buffer, strlen(buffer));
		HAL_UART_Transmit_IT(&huart4, buffer, strlen(buffer));
		HAL_UART_Transmit_IT(&huart5, buffer, strlen(buffer));
		HAL_UART_Transmit_IT(&huart10, buffer, strlen(buffer));

	}

	//return temperature;

	//}

}
#endif


#if 1

float ADC_READ(uint8_t adc_num,uint8_t channel)
{
	uint8_t txd_buf[30]={0};
	uint8_t rxd_buf[30]={0};
	float resistance = 0;
	//	//GPIO_TypeDef *GPIO;
	//	static int wait_time = 10;
	//	uint8_t txd_buf[30] = { 0 };
	//	uint8_t rxd_buf[30] = { 0 };
	//	uint16_t reg_data;//[4]= {0};
	//	uint8_t res;
	//	uint8_t read_register = 0;
	//	uint8_t write_reagister = 0;

	txd_buf[0] = 0x40;
	txd_buf[1] = 0x11;
	txd_buf[2] = 0x0c; // WREG write AIN10-AINCOM,(1010 1100) binary,MUXP and MUXN selection
	txd_buf[3] = 0x80; // gain =1
	txd_buf[4] = 0x76; // DATA RATE 20 SPS
	txd_buf[5] = 0xEA; //REF on (0001 10-ref 01)
	txd_buf[6] = 0x1E; // IDAC MAG
	txd_buf[7] = 0x02; // IDAC MUX
	txd_buf[8] = 0x07; // Vbias
	txd_buf[9] = 0xfb; // Timeout bit second =1 ,default is 10h
	txd_buf[10] = 0x00; // start converting command
	txd_buf[11] = 0x10; // start converting command
	txd_buf[12] = 0x0; // start converting command
	txd_buf[13] = 0x0; // start converting command
	txd_buf[14] = 0x0; // start converting command
	txd_buf[15] = 0x0; // start converting command
	txd_buf[16] = 0x0; // start converting command
	txd_buf[17] = 0x40; // start converting command
	txd_buf[18] = 0x00; // start converting command
	txd_buf[19] = 0x00; // start converting command
	txd_buf[20] = 0x00; // start converting command
	txd_buf[18] = 0x00; // start converting command

	//	txd_buf[0] = 0x40;
	//	txd_buf[1] = 0x11;
	//	txd_buf[2] = 0x0c; // WREG write AIN10-AINCOM,(1010 1100) binary,MUXP and MUXN selection
	//	txd_buf[3] = 0x80; // gain =1
	//	txd_buf[4] = 0x76; // DATA RATE 20 SPS
	//	txd_buf[5] = 0x0A; //REF on (0001 10-ref 01)
	//	txd_buf[6] = 0x14; // IDAC MAG
	//	txd_buf[7] = 0x02; // IDAC MUX
	//	txd_buf[8] = 0x07; // Vbias
	//	txd_buf[9] = 0xfb; // Timeout bit second =1 ,default is 10h
	//	txd_buf[10] = 0x00; // start converting command
	//	txd_buf[11] = 0x10; // start converting command
	//	txd_buf[12] = 0x0; // start converting command
	//	txd_buf[13] = 0x0; // start converting command
	//	txd_buf[14] = 0x0; // start converting command
	//	txd_buf[15] = 0x0; // start converting command
	//	txd_buf[16] = 0x0; // start converting command
	//	txd_buf[17] = 0x40; // start converting command
	//	txd_buf[18] = 0x00; // start converting command
	//	txd_buf[19] = 0x00; // start converting command
	//	txd_buf[20] = 0x00; // start converting command
	//	txd_buf[18] = 0x00; // start converting command

	switch(channel)
	{
	case 1:
		txd_buf[4] = 0x43; // DATA RATE 20 SPS
		txd_buf[7] = 0x02; // IDAC MUX
		txd_buf[9] = 0xf5; // Timeout bit second =1 ,default is 10h
		break;

	case 2:
		txd_buf[4] = 0x10; // DATA RATE 20 SPS
		txd_buf[7] = 0x02; // IDAC MUX
		txd_buf[9] = 0xf2; // Timeout bit second =1 ,default is 10h
		break;

	case 3:
		txd_buf[4] = 0x9A; // DATA RATE 20 SPS
		txd_buf[7] = 0x02; // IDAC MUX
		txd_buf[9] = 0xf8; // Timeout bit second =1 ,default is 10h
		break;
	case 4:
		txd_buf[4] = 0x76; // DATA RATE 20 SPS
		txd_buf[7] = 0x02; // IDAC MUX
		txd_buf[9] = 0xfb; // Timeout bit second =1 ,default is 10h
		break;
	}

	switch(adc_num)
	{
	case 1:

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
		//HAL_SPI_Receive(&hspi5, (uint8_t*) &rxd_buf, 20, 0xFF);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);


		//HAL_Delay(200);

		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		txd_buf[0] = 0x20;
		txd_buf[1] = 0x11;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf, 2, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi5, (uint8_t*) &rxd_buf, 0x11, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);
		//		if(rxd_buf[7]==txd_buf[9])
		//		{
		//			HAL_Delay(1);
		//			sprintf(buffer, "S  %d  %dE\n\r",adc_num,channel);
		//					HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		}
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



		//				while(1)
		//				{

		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 0);
		txd_buf[0] = 0x13;
		//HAL_Delay(2);
		HAL_SPI_Transmit(&hspi5, (uint8_t*) &txd_buf, 1, 0xFF);
		//HAL_Delay(2);
		HAL_SPI_Receive(&hspi5, (uint8_t*) &reg_data_ADC_1, 0x2, 0xFF);

		reg_data_ADC_1 = (reg_data_ADC_1 >> 8 | reg_data_ADC_1 << 8 ) ;
		HAL_GPIO_WritePin(GPIOF, CS_1_Pin, 1);

		resistance = ((2*1000*(float)reg_data_ADC_1)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;

		if(rxd_buf[7]==txd_buf[9] )
		{
			HAL_Delay(1);
			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_1,temperature,resistance);
			//HAL_UART_Transmit_IT(&huart5, buffer, sizeof(buffer));
		}
		if(channel == 1)
		{
			temp_sensor_1 = temperature ;

		}
		else if(channel == 2)
		{
			temp_sensor_2 = temperature ;

		}
		else if(channel == 3)
		{
			temp_sensor_3 = temperature ;

		}
		else if(channel == 4)
		{
			temp_sensor_4 = temperature ;

		}
		//HAL_Delay(100000000);

		break;

	case 2:
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
		//HAL_SPI_Receive(&hspi2, (uint8_t*) &rxd_buf, 20, 0xFF);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);


		//HAL_Delay(200);

		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		txd_buf[0] = 0x20;
		txd_buf[1] = 0x11;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf, 2, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi2, (uint8_t*) &rxd_buf, 0x11, 0xFF);
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);

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



		//				while(1)
		//				{

		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 0);
		txd_buf[0] = 0x13;
		HAL_Delay(2);
		HAL_SPI_Transmit(&hspi2, (uint8_t*) &txd_buf, 1, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi2, (uint8_t*) &reg_data_ADC_2, 0x2, 0xFF);
		reg_data_ADC_2 = (reg_data_ADC_2 >> 8 | reg_data_ADC_2 << 8 ) ;
		HAL_Delay(2);
		HAL_GPIO_WritePin(GPIOB, CS_2_Pin, 1);
		resistance = ((2*1000*(float)reg_data_ADC_2)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		if(rxd_buf[7]==txd_buf[9] )
		{
			HAL_Delay(1);
			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_2,temperature,resistance);
			//HAL_UART_Transmit_IT(&huart5, buffer, sizeof(buffer));
		}
		if(channel == 1)
		{
			temp_sensor_5 = temperature ;

		}
		else if(channel == 2)
		{
			temp_sensor_6 = temperature ;

		}
		else if(channel == 3)
		{
			temp_sensor_7 = temperature ;

		}
		else if(channel == 4)
		{
			temp_sensor_8 = temperature ;

		}
		break;

	case 3:
		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 0);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA, RESET_3_Pin, 1);
		//HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		HAL_GPIO_WritePin(GPIOA, START_3_Pin, 0);


		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);


		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf,0x13, 0xFF);
		HAL_Delay(2);
		//HAL_SPI_Receive(&hspi3, (uint8_t*) &rxd_buf, 20, 0xFF);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);


		//HAL_Delay(200);

		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		txd_buf[0] = 0x20;
		txd_buf[1] = 0x11;
		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf, 2, 0xFF);
		HAL_Delay(2);
		HAL_SPI_Receive(&hspi3, (uint8_t*) &rxd_buf, 0x11, 0xFF);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);

		//		if(rxd_buf[7]==txd_buf[9])
		//		{
		//			HAL_Delay(1);
		//			sprintf(buffer, "S  %d  %dE\n\r",adc_num,channel);
		//					HAL_UART_Transmit_IT(&huart8, buffer, sizeof(buffer));
		//		}
		HAL_GPIO_WritePin(GPIOA, START_3_Pin, 1);

		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		txd_buf[0] = 0x19;

		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf, 1, 0xFF);
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
		HAL_Delay(wait_time);

		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 0);
		txd_buf[0] = 0x13;

		HAL_SPI_Transmit(&hspi3, (uint8_t*) &txd_buf, 1, 0xFF);

		HAL_SPI_Receive(&hspi3, (uint8_t*) &reg_data_ADC_3, 0x2, 0xFF);
		reg_data_ADC_3 = (reg_data_ADC_3 >> 8 | reg_data_ADC_3 << 8 ) ;
		HAL_GPIO_WritePin(GPIOD, CS_3_Pin, 1);
		resistance = ((2*1000*(float)reg_data_ADC_3)/4) / 0xFFFF;
		temperature = (resistance - 100.0 ) / 0.385;
		if(rxd_buf[7]==txd_buf[9] )
		{
			HAL_Delay(1);
			sprintf(buffer, "S  %d  %dE  %X  temp = %f  resis == %f\n\r",adc_num,channel,reg_data_ADC_3,temperature,resistance);
			//HAL_UART_Transmit_IT(&huart5, buffer, sizeof(buffer));
		}
		if(channel == 1)
		{
			temp_sensor_9 = temperature ;

		}
		else if(channel == 2)
		{
			temp_sensor_10 = temperature ;

		}
		else if(channel == 3)
		{
			temp_sensor_11 = temperature ;

		}
		else if(channel == 4)
		{
			temp_sensor_12 = temperature ;

		}
		break;
	}


	if(channel == 4 && adc_num == 3)
	{
		uint8_t time_data[8]={0};
		uint32_t sec_addr = 0x60000000 | 0x7FFF9;
		HAL_SRAM_Read_8b(&hsram1, (uint32_t *)sec_addr, (uint8_t *)&time_data,7);
		memset(buffer_1,0,sizeof(buffer));
		HAL_Delay(1);
		sprintf(buffer_1,
				"{\"TS1\": \"%.2f\", \"TS2\": \"%.2f\", \"TS3\": \"%.2f\", \"TS4\": \"%.2f\", \"TS5\": \"%.2f\", "
				"\"TS6\": \"%.2f\", \"TS7\": \"%.2f\", \"TS8\": \"%.2f\", \"TS9\": \"%.2f\", \"TS10\": \"%.2f\", "
				"\"TS11\": \"%.2f\", \"TS12\": \"%.2f\"}",// \"TIME\": \"%x:%x:%x\",\"DATE\": \"%x-%x-20%x}\"\n\r",
				temp_sensor_1, temp_sensor_2, temp_sensor_3, temp_sensor_4, temp_sensor_5, temp_sensor_6,
				temp_sensor_7, temp_sensor_8, temp_sensor_9, temp_sensor_10, temp_sensor_11, temp_sensor_12,time_data[2],time_data[1],time_data[0],time_data[4],time_data[5],time_data[6]);
		if(1)
		{
//			HAL_UART_Transmit_IT(&huart8, buffer_1, strlen(buffer_1));

//			HAL_GPIO_WritePin(GPIOA, RS485_CTRL2_Pin, GPIO_PIN_SET);
//			HAL_Delay(10);
//			HAL_UART_Transmit_IT(&huart4, buffer_1, strlen(buffer_1));

			HAL_UART_Transmit_IT(&huart5, buffer_1, strlen(buffer_1));
//			HAL_UART_Transmit_IT(&huart10, buffer, strlen(buffer));
			HAL_Delay(10);
		}

	}

	//HAL_Delay(500);

	return temperature;

	//}

}
#endif
