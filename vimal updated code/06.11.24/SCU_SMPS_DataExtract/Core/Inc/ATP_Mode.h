/*
 * ATP_Mode.h
 *
 *  Created on: Nov 6, 2024
 *      Author: AEM
 */

#ifndef INC_ATP_MODE_H_
#define INC_ATP_MODE_H_


#define SoF					'@'
#define EoF					'$'


extern uint8_t ATP_UART;


#if ATP_UART
#define ADC_UART
#endif

#if 0
#define NandUART
#endif


//extern uint8_t AdcSensor_Avg(uint16_t,uint8_t, uint8_t);
//extern float ADC_READ(uint8_t adc_num,uint8_t channel);
//extern void time_setter( );
//extern void ATP_operation();
//extern uint8_t ADC_operation();
//extern void NAND_operation(void);
//extern uint8_t SRAM_operation();
//extern uint8_t get_NAND_address(void);

extern NAND_AddressTypeDef nand_address;
extern NAND_HandleTypeDef hnand1;
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
	channel_1,
	channel_2,
	channel_3,
	channel_4,
}adcchannel;


typedef enum
{
	adc_1,
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


#endif /* INC_ATP_MODE_H_ */
