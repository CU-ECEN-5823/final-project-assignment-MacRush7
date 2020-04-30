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
 * Project Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file main_app.c
 *
 * @brief Main application source file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/main_app.h>

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************//**
 * This function initialises the basic EFR32BG13 peripherals used in this
 * project.
 ******************************************************************************/

void gecko_system_init(void)
{
	/* Initializing Peripherals and Configurations */
	sm_Init();
	gpioInit();
	pushButton_Init();
	cmu_Init();
	letimer_Init();
	i2c_Init();
	logInit();
	displayInit();
}

/***************************************************************************//**
 * This function handles the BTM external events occurring with the MCP9808
 * temperature state machine.
 ******************************************************************************/

void gecko_external_evt_handler(void)
{
	#ifdef MCP9808_ENABLED

	{
		CORE_DECLARE_IRQ_STATE;
		CORE_ENTER_CRITICAL();
		{
		  event_processing_bitmask |= interrupt_event_bitmask;
		  EVT_PR_BITM_CLEAR_EXTRA();
		  sm_ClearInt();
		}
		CORE_EXIT_CRITICAL();

		if(event_processing_bitmask)
		{
		  static int i = 0;
		  for(i = 0; event_processing_bitmask != 0; i++)
			  sm_HandleEvents(i);
		}
	}

	#endif
}
