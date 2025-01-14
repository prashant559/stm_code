/*
 * SRAM_Operations.h
 *
 *  Created on: Nov 6, 2024
 *      Author: AEM
 */

#ifndef INC_SRAM_OPERATIONS_H_
#define INC_SRAM_OPERATIONS_H_


typedef struct{

	uint8_t Startbyte;
	uint8_t SRAM_buffer[350];
	uint8_t Endbyte;
	uint8_t SRAM_flag;
	uint16_t Idxcount;
}_sSRAMOperation;

#endif /* INC_SRAM_OPERATIONS_H_ */
