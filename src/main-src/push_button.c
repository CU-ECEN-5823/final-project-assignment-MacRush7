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

/* @file push_button.c
 *
 * @brief On-board push-button handling source file.
 *
 * @author Rushi James Macwan
 *
 * Reference: soc-btmesh-switch example
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/push_button.h>

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief PUSHBUTTON Initialisation function
 *
 * Function overview
 * Initialises the pushbutton pin, resets its state and enables pushbutton interrupts.
 *
 * @param void
 * @return void.
 */

void pushButton_Init(void)
{
	/*
	 * Adding configuration for GPIO push button PB0
	 */

	GPIO_PinModeSet(PB0_PORT, PB0_PIN, gpioModeInputPullFilter, true);
	GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInputPullFilter, true);

	/* Resetting pushbutton initial state. */
	pushButton_Reset();
}

/**
 * @brief PUSHBUTTON Enabling interrupts.
 *
 * Function overview
 * Function to configure and enable pushbutton interrupts with callback
 * to IRQHandler().
 *
 * @param void
 * @return void.
 */

void pushButton_EnableInt(void)
{
	GPIOINT_Init();

	/* configure interrupt for PB0 and PB1, both falling and rising edges */
	GPIO_ExtIntConfig(PB0_PORT, PB0_PIN, PB0_PIN, true, true, true);
	GPIO_ExtIntConfig(PB1_PORT, PB1_PIN, PB1_PIN, true, true, true);

	/* register the callback function that is invoked when interrupt occurs */
	GPIOINT_CallbackRegister(PB0_PIN, pushButton_IRQHandler);
	GPIOINT_CallbackRegister(PB1_PIN, pushButton_IRQHandler);
}

/**
 * @brief PUSHBUTTON Reset function
 *
 * Function overview
 * Resets pushbutton initial state.
 *
 * @param void
 * @return void.
 */

void pushButton_Reset(void)
{
	PB0_val = BUTTON_RELEASED;
	PB1_val = BUTTON_RELEASED;
}

/**
 * @brief PUSHBUTTON IRQ handler
 *
 * Function overview
 * Handles pushbutton interrupts based on the button used
 * for interaction.
 *
 * @param uint8_t pin
 * @return void.
 */

void pushButton_IRQHandler(uint8_t pin)
{
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();

	if(pin == PB0_PIN)
	{
		/* PB0 Pressed */
		if(GPIO_PinInGet(PB0_PORT, PB0_PIN) == 0)
		{
			PB0_val = BUTTON_PRESSED;
			gecko_external_signal(EXT_SIGNAL_PB0_PRESSED);
		}

		/* PB0 Released */
		else
		{
			PB0_val = BUTTON_RELEASED;
			gecko_external_signal(EXT_SIGNAL_PB0_RELEASED);
		}
	}

	if(pin == PB1_PIN)
	{
		/* PB1 Pressed */
		if(GPIO_PinInGet(PB1_PORT, PB1_PIN) == 0)
		{
			PB1_val = BUTTON_PRESSED;
		}

		/* PB1 Released */
		else
		{
			PB1_val = BUTTON_RELEASED;
		}
	}

	CORE_EXIT_CRITICAL();
}
