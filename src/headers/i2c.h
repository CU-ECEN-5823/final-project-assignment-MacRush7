/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file i2c.h
 *
 * @brief I2C unit header file.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_HEADERS_I2C_H_
#define SRC_HEADERS_I2C_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include "header.h"

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

volatile float temp_reading;
int32_t app_temp_reading;
volatile uint8_t connect_flag;
volatile uint8_t read_flag;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void i2c_Init(void);
void I2C_Write(uint8_t addr, uint8_t command);
void I2C_Read(uint8_t addr);
void I2C0_IRQHandler(void);

#endif /* SRC_HEADERS_I2C_H_ */
