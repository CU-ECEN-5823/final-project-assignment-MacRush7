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

/* @file cmu.c
 *
 * @brief Clock management unit source file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/cmu.h>

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief CMU initialization
 *
 * Function overview
 * Function selects clocks, prescales and enables it for required peripherals.
 *
 * @param void
 * @return  void.
 */

void cmu_Init(void)
{
	cmu_ClkSelect();
	cmu_ClkEnable();
}

/**
 * @brief CMU clock select
 *
 * @param void
 * @return  void.
 */

void cmu_ClkSelect(void)
{
	/* Selecting ULFRCO clock to allow EM3 sleeping. */
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
}

/**
 * @brief CMU clock enable for required peripherals
 *
 * @param void
 * @return  void.
 */

void cmu_ClkEnable(void)
{
	// Enabling clock to Core LE peripherals:
	CMU_ClockEnable(cmuClock_CORELE, true);

	// Enabling clock to GPIO module:
	CMU_ClockEnable(cmuClock_GPIO, true);

	// Enabling clock to LETIMER0 module:
	CMU_ClockEnable(cmuClock_LETIMER0, true);
}
