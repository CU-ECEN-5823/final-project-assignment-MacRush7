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

/* @file state.c
 *
 * @brief State machine source file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/state.h>

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief State Machine Clear interrupts in the interrupt_event_bitmask variable
 *
 * Function overview
 * Handles state machine defined interrupts.
 *
 * @param void
 * @return  void.
 */

void sm_ClearInt(void)
{
	/* STATE0_MCP9808_TRANSACTION_START_EVENT */
	if(LETIMER_WAIT_FLAG_CHECK())
	{
		LETIMER_WAIT_FLAG_CLEAR();
	}

	/* STATE1_MCP9808_I2C_WRITE_COMPLETED_EVENT */
	if(TEMP_I2C_WRITE_FLAG_CHECK())
	{
		TEMP_I2C_WRITE_FLAG_CLEAR();
	}

	/* STATE2_MCP9808_I2C_READ_COMPLETED_EVENT */
	if(TEMP_I2C_READ_FLAG_CHECK())
	{
		TEMP_I2C_READ_FLAG_CLEAR();
	}
}

/**
 * @brief State Machine Handle interrupt events based on event_processing_bitmask value
 *
 * Function overview
 * Schedules events and handles current event.
 *
 * @param int current_state
 * @return  void.
 */

void sm_HandleEvents(int current_state)
{
	sm_HandleReset(current_state);

	static char LCD_print[30];

	switch(current_state)
	{
		case 0:
			/* STATE0_MCP9808_TRANSACTION_START_EVENT */
			if(P_LETIMER_WAIT_FLAG_CHECK())
			{
				sm_ReportState(current_state);
				P_LETIMER_WAIT_FLAG_CLEAR();
				mcp9808_Write();
			}
			break;

		case 1:
			/* STATE1_MCP9808_I2C_WRITE_COMPLETED_EVENT */
			if(P_TEMP_I2C_WRITE_FLAG_CHECK())
			{
				sm_ReportState(current_state);
				P_TEMP_I2C_WRITE_FLAG_CLEAR();
				mcp9808_Read();
			}
			break;

		case 2:
			/* STATE2_MCP9808_I2C_READ_COMPLETED_EVENT */
			if(P_TEMP_I2C_READ_FLAG_CHECK())
			{
				sm_ReportState(current_state);
				P_TEMP_I2C_READ_FLAG_CLEAR();
				logTemp();
				sprintf(LCD_print, "%s %f", "Temp(C): ", temp_reading);
				displayPrintf(DISPLAY_ROW_BTADDR2, LCD_print);
			}
			break;

		default:
			/* Reset the state machine */
			sm_HandleReset(current_state);
			break;
	}
}

/**
 * @brief State Machine Report current state of the state machine
 *
 * @param int current_state
 * @return  const char.
 */

const char *getStateReport(int current_state)
{
	switch(current_state)
	{
		case 0:
			return "STATE0_MCP9808_TRANSACTION_START_EVENT";
			break;

		case 1:
			return "STATE1_MCP9808_I2C_WRITE_COMPLETED_EVENT";
			break;

		case 2:
			return "STATE2_MCP9808_I2C_READ_COMPLETED_EVENT";
			break;

		default:
			return "STATE UNDEFINED";
			break;
	}
}

/**
 * @brief State Machine Clear interrupt flags
 *
 * @param void
 * @return void.
 */

void sm_Init(void)
{
	/* Configuration for: Interrupt event bitmask OR Event processing bitmask
	 *
	 * 		0b | 0 0 0 0 | 0 0 0 0 |
	 *  Bits ->  7 6 5 4   3 2 1 0
	 *
	 *		Flag Type						Bit			Set Condition (1)	Reset Condition (0)
	 *
	 *      LETIMER_WAIT_FLAG				-> 0		-> |= 0x01			-> &= 0xFE
	 *      LPM_WARMUP_FLAG			 		-> 1		-> |= 0x02			-> &= 0xFD
	 *      TEMP_I2C_WRITE_FLAG		 		-> 2		-> |= 0x04			-> &= 0xFB
	 *      TEMP_I2C_READ_DELAY_FLAG		-> 3		-> |= 0x08			-> &= 0xF7
	 *      TEMP_I2C_READ_FLAG				-> 4		-> |= 0x10			-> &= 0xEF
	 *
	 *      LPM_WARMUP_START_FLAG			-> 5		-> |= 0x20			-> &= 0xDF
	 *      TEMP_I2C_READ_DELAY_START_FLAG	-> 6		-> |= 0x40  		-> &= 0xBF
	 *
	 *
	 */

	event_processing_bitmask = 0;
	interrupt_event_bitmask = 0;
}

/**
 * @brief State Machine Handle reset events
 *
 * @param int current_state
 * @return void.
 */

void sm_HandleReset(int current_state)
{
	if(current_state > 2 || current_state < 0)
	{
		current_state = 0;
	}
}

/**
 * @brief State Machine Report current state
 *
 * @param int current_state
 * @return void.
 */

void sm_ReportState(int current_state)
{
	#ifdef LOG_STATUS
	logSM_Status(current_state);
	#endif
}
