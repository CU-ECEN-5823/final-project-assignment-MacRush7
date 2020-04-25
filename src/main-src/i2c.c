/*******************************************************************************
 * # License
 * <b>Copyright 2016 Silicon Laboratories, Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file i2c.c
 *
 * @brief I2C unit source file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/i2c.h>
#include "i2cspm.h"
#include "native_gecko.h"

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

I2CSPM_Init_TypeDef i2cInit =
{
		.port = I2C0,
		.sclPort = gpioPortC,
		.sclPin = 10,
		.sdaPort = gpioPortC,
		.sdaPin = 11,
		.i2cRefFreq = 0,
		.i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
		.i2cClhr = i2cClockHLRStandard,
		.portLocationScl = 14,
		.portLocationSda = 16
};

volatile I2C_TransferReturn_TypeDef transferStatusConnect;
volatile I2C_TransferReturn_TypeDef transferStatusRead;

volatile uint8_t connect_flag;
volatile uint8_t read_flag;

I2C_TransferSeq_TypeDef    seq;
uint8_t i2c_read_data[2];
uint8_t i2c_write_data[1];

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief I2C initialization
 *
 * Function overview
 * Initializes the I2C peripheral for connection with the Si7021 Temperature-Humidity sensor.
 *
 * @param void
 * @return  void.
 */

void i2c_Init(void)
{
	I2CSPM_Init(&i2cInit);
}

/**
 * @brief I2C Connect Function
 *
 * Function overview
 * For writing to the I2C Slave device.
 *
 * @param uint8_t addr, uint8_t command
 * @return  void.
 *
 * https://www.silabs.com/community/mcu/32-bit/forum.topic.html/interrupt-driveni2c-FM0S
 */

void I2C_Write(uint8_t addr, uint8_t command)
{
	addr 	= addr << 1;
	i2c_write_data[0] = command;

	seq.addr = addr;
	seq.flags = I2C_FLAG_WRITE;
	seq.buf[0].data = i2c_write_data;
	seq.buf[0].len = 1;

	connect_flag = 1;
	NVIC_EnableIRQ(I2C0_IRQn);
	transferStatusConnect = I2C_TransferInit(I2C0, &seq);
	SLEEP_SleepBlockBegin(sleepEM2);
}

/**
 * @brief I2C Read Function
 *
 * Function overview
 * For reading from the I2C Slave device.
 *
 * @param uint8_t addr
 * @return  void.
 *
 * https://www.silabs.com/community/mcu/32-bit/forum.topic.html/interrupt-driveni2c-FM0S
 */

void I2C_Read(uint8_t addr)
{
	addr = addr << 1;
	addr |= 0x01;

	seq.addr  = addr;
	seq.flags = I2C_FLAG_READ;
	seq.buf[0].data = i2c_read_data;
	seq.buf[0].len  = 2;

	read_flag = 1;
	NVIC_EnableIRQ(I2C0_IRQn);
	transferStatusRead = I2C_TransferInit(I2C0, &seq);
	SLEEP_SleepBlockBegin(sleepEM2);
}

/*
 * @brief I2C (Interrupt Request) Handler
 *
 * Function overview
 * Handles interrupts for I2C Transfers
 *
 * @param void
 * @return  void.
 */

void I2C0_IRQHandler(void)
{
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();

	if(connect_flag)
	{
		transferStatusConnect = I2C_Transfer(I2C0);
		if(transferStatusConnect != i2cTransferInProgress)
		{
			if(transferStatusConnect == i2cTransferDone)
			{
				SLEEP_SleepBlockEnd(sleepEM2);
				NVIC_DisableIRQ(I2C0_IRQn);

				TEMP_I2C_WRITE_FLAG_SET();
				gecko_external_signal(GECKO_TEMP_I2C_WRITE_FLAG);

				connect_flag = 0;
			}
			else
			{
				int status = transferStatusConnect;
				logI2CWriteReturns(status);
			}
		}
	}
	else if(read_flag)
	{
		transferStatusRead	= I2C_Transfer(I2C0);
		if(transferStatusRead != i2cTransferInProgress)
		{
			if(transferStatusRead == i2cTransferDone)
			{
				SLEEP_SleepBlockEnd(sleepEM2);
				NVIC_DisableIRQ(I2C0_IRQn);

				uint8_t LSB = i2c_read_data[1];
				uint8_t MSB = i2c_read_data[0];

				MSB &= 0x1F;

				// Negative temperature reading
				if((MSB & 0x10) == 0x10)
				{
					MSB &= 0x0F;
					temp_reading = 256 - (((float) MSB * 16) + ((float) LSB / 16));
				}

				// Positive temperature reading
				else
				{
					temp_reading = (((float) MSB * 16) + ((float) LSB / 16));
				}

				app_temp_reading = temp_reading * 1000;

				TEMP_I2C_READ_FLAG_SET();
				gecko_external_signal(GECKO_TEMP_I2C_READ_FLAG);

				read_flag = 0;
			}
			else
			{
				int status = transferStatusRead;
				logI2CReadReturns(status);
			}
		}
	}

	CORE_EXIT_CRITICAL();
}
