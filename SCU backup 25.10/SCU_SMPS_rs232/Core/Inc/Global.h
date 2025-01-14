/*
 * Global.h
 *
 *  Created on: Oct 22, 2024
 *      Author: AEM
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_


#define ADC_MAX_VAL			26500
#define ADC_MIN_VAL 		8188
#define ADCsample  			10
#define Wirebrake  			6
#define SoF					'@'
#define EoF					'$'
#define NAND_PAGE_SIZE 		8192

extern uint8_t ATP_UART;


#if ATP_UART
#define ADC_UART
#endif

#if 0
#define NandUART
#endif


extern uint8_t AdcSensor_Avg(uint16_t,uint8_t, uint8_t);
extern float ADC_READ(uint8_t adc_num,uint8_t channel);
extern void time_setter( );
extern void ATP_operation();
extern uint8_t ADC_operation();
extern void NAND_operation(void);
extern uint8_t SRAM_operation();
extern uint8_t get_NAND_address(void);
extern SRAM_HandleTypeDef hsram1;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart10;

typedef enum
{
	LOW,
	HIGH,
}status;

/*
 * This is ADC channels,
 * using in AdcSensor_Avg() function
 */
typedef enum
{
	channel_1=1,
	channel_2,
	channel_3,
	channel_4,
}adcchannel;


typedef enum
{
	adc_1=1,
	adc_2,
	adc_3,
}adc;


typedef enum
{
	POWER_CTRL_1=1,
	POWER_CTRL_2,
	INPUT_GPIO_1,
	INPUT_GPIO_2,
	INPUT_GPIO_3,
	INPUT_GPIO_4,
	INPUT_GPIO_5,
	OUTPUT_GPIO_1,
	OUTPUT_GPIO_2,
	TS_1,
	TS_2,
	TS_3,
	TS_4,
	TS_5,
	TS_6,
	TS_7,
	TS_8,
	TS_9,
	TS_10,
	TS_11,
	TS_12,
	RTC_CHECK,
	NAND_FLASH_WRITE,
	NAND_FLASH_READ,
	NAND_FLASH_ERASE,
	LoopBack_RS232,
	LoopBack_RS422,
	LoopBack1_RS485,
	LoopBack2_RS485,

}ATP_Operation;


/*
 * structure is using for avg calculation,
 * in AdcSensor_Avg() function
 */
typedef struct{

	uint16_t channel[ADCsample];
	uint8_t Index;
	uint32_t sum;
	uint16_t average;
	uint8_t count;
}_sADCavgcal;


typedef struct{

	_sADCavgcal ADCavgcal_1;
	_sADCavgcal ADCavgcal_2;
	_sADCavgcal ADCavgcal_3;
	_sADCavgcal ADCavgcal_4;

}_sADCmodule;


/*
 * Frame of ADC value
 */
typedef struct{

	char Startbyte;
	char adcbuffer[300];
	char Endbyte;
	size_t dataSize;
}_sADCpacket;


typedef struct{

	uint8_t Startbyte;
	uint8_t NAND_write[8195];
	uint8_t NAND_read[8195];
	uint8_t Endbyte;
	uint8_t Nand_flag;
}_sNandOperation;


typedef struct{

	uint8_t Startbyte;
	uint8_t SRAM_buffer[300];
	uint8_t Endbyte;
	uint8_t SRAM_flag;
	uint8_t Idxcount;
}_sSRAMOperation;


#endif /* INC_GLOBAL_H_ */
