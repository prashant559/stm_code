/*
 * NAND_operations.c
 *
 * Created on: Sep 9, 2024
 * Author: VIMAL
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

// External handles for NAND and UART
extern NAND_HandleTypeDef hnand1;
extern UART_HandleTypeDef huart5;

// Global NAND configuration and data buffers
NAND_IDTypeDef nand_details;
NAND_AddressTypeDef nand_address = {0, 0, 0};
uint8_t read_data[8192] = {0};
uint8_t NAND_data[8192] = {0};
uint8_t spare_buffer[448] = {0};
uint8_t nand_block_status[4096] = {0};

uint32_t page_read = 0;
uint32_t block_read = 0;
uint32_t plane_read = 0;
uint8_t nand_update = 0;
uint8_t nand_cyclic_run = 0;

/**
 * @brief Performs NAND operations like writing, reading, and checking block status.
 * This function writes data to NAND, reads it back to verify, checks and updates
 * block status based on the comparison result.
 */
void NAND_operation(void)
{
    HAL_GPIO_WritePin(GPIOE, LED1_Pin, 1); // Toggle LED for visual feedback
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOE, LED1_Pin, 0);
    HAL_Delay(1);

    if (nand_update)
    {
        // Prepare data for NAND write
        memset(read_data, 0, 8192);
        nand_update = 0;
        get_NAND_data(NAND_data);

        // Write data to NAND flash
        HAL_NAND_Write_Page_8b(&hnand1, &nand_address, NAND_data, 1);

        // Read data back from NAND flash for verification
        HAL_NAND_Read_Page_8b(&hnand1, (const NAND_AddressTypeDef *) &nand_address, read_data, 1);

        // Calculate bit difference between written and read data
        uint16_t bit_difference = calculate_hamming_distance(NAND_data, read_data, 8192);

        // Check if data read matches data written
        if (bit_difference <50) // Data matches
        {
            if (nand_address.Page == 0)
            {
                set_NAND_block_status(nand_address.Page, nand_address.Block, nand_address.Plane, 1); // Mark block as good
            }
            nand_address.Page++;
        }
        else // Data mismatch - possible bad block
        {
            handle_bad_block(); // Call function to handle the bad block scenario
        }

        // Move to the next block if all pages in the current block have been written
        if (nand_address.Page > 127)
        {
            nand_address.Block++;
            nand_address.Page = 0;
            HAL_NAND_Erase_Block(&hnand1, &nand_address);
            HAL_Delay(100);
        }
    }
}

/**
 * @brief Reads and updates the status of each block in NAND.
 * Sets the block status array based on whether the block is good (0xFF) or bad (0x00).
 */
void get_NAND_block_status()
{
    for (uint16_t i = 0; i < 4096; i++)
    {
        nand_address.Block = i;
        HAL_NAND_Read_SpareArea_8b(&hnand1, &nand_address, spare_buffer, 1);
        nand_block_status[i] = (spare_buffer[0] == 0xFF) ? 1 : 0;
    }
}

/**
 * @brief Sets the status of a NAND block by updating its spare area.
 * @param page The page number within the block.
 * @param block The block number to update.
 * @param plane The NAND plane number.
 * @param block_state 1 for good block, 0 for bad block.
 */
void set_NAND_block_status(uint16_t page, uint16_t block, uint16_t plane, uint8_t block_state)
{
    NAND_AddressTypeDef nand_address_block = {0, block, plane};
    HAL_NAND_Read_SpareArea_8b(&hnand1, &nand_address_block, spare_buffer, 1);

    spare_buffer[0] = (block_state) ? 0xFF : 0x00;  // Set good (0xFF) or bad (0x00)
    nand_block_status[block] = block_state;          // Update block status array
    HAL_NAND_Write_SpareArea_8b(&hnand1, &nand_address_block, spare_buffer, 1);
}

/**
 * @brief Counts the number of set bits (1s) in a byte.
 * @param byte The byte to analyze.
 * @return The number of set bits in the byte.
 */
int count_set_bits(uint8_t byte)
{
    int count = 0;
    while (byte)
    {
        count += byte & 1;
        byte >>= 1;
    }
    return count;
}

/**
 * @brief Calculates the Hamming distance (bit differences) between two 8192-byte strings.
 * @param str1 Pointer to the first 8192-byte string.
 * @param str2 Pointer to the second 8192-byte string.
 * @param length Length of the strings to compare (8192 bytes).
 * @return The number of differing bits between the two strings.
 */
int calculate_hamming_distance(const uint8_t *str1, const uint8_t *str2, size_t length)
{
    int differing_bits = 0;
    for (size_t i = 0; i < length; i++)
    {
        differing_bits += count_set_bits(str1[i] ^ str2[i]);  // XOR and count set bits in result
    }
    return differing_bits;
}

/**
 * @brief Handles bad block processing in NAND by erasing and relocating data.
 * If data verification fails, the function marks the block as bad, and attempts to write data
 * to the next available block.
 */
void handle_bad_block(void)
{
    nand_address.Page = 0;
    HAL_NAND_Erase_Block(&hnand1, &nand_address);
    HAL_Delay(1000);

    // Attempt to rewrite data after erasing the block
    HAL_NAND_Write_Page_8b(&hnand1, &nand_address, NAND_data, 1);
    HAL_NAND_Read_Page_8b(&hnand1, (const NAND_AddressTypeDef *)&nand_address, read_data, 1);

    // Verify written data again
    if (calculate_hamming_distance(NAND_data, read_data, 8192) < 50) // Data matches
    {
        if (nand_address.Page == 0)
        {
            set_NAND_block_status(nand_address.Page, nand_address.Block, nand_address.Plane, 1);
        }
        nand_address.Page++;
    }
    else // Mark block as bad and attempt to move data
    {
        set_NAND_block_status(nand_address.Page, nand_address.Block, nand_address.Plane, 0); // Mark as bad
        nand_address.Page = 0;

        // Move to the next block based on current status
        if (nand_block_status[nand_address.Block + 1] == 1)
        {
            nand_address.Block++;
            handle_bad_block();
        }
        else
        {
            nand_address.Block += 2; // Skip bad block and move further
            handle_bad_block();
        }

        // Handle block wrapping around in case of overflow
        if (nand_address.Block > 4095)
        {
            nand_address.Block = 0;
            nand_cyclic_run = 1;
        }

       // HAL_NAND_Erase_Block(&hnand1, &nand_address);  // Erase new block
        HAL_Delay(1000);
    }
}
