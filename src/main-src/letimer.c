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

/* @file letimer.c
 *
 * @brief Low energy timer LETIMER source file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/letimer.h>

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

const LETIMER_Init_TypeDef letimerInit =
{
		.enable = true,
		.debugRun = true,
		.comp0Top = true,
		.bufTop = false,
		.out0Pol = 0,
		.out1Pol = 0,
		.ufoa0 = letimerUFOANone,
		.ufoa1 = letimerUFOANone,
		.repMode = letimerRepeatFree
};

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief LETIMER0 IRQ (Interrupt Request) Handler
 *
 * Function overview
 * Handles interrupts for COMP0, COMP1 and UF.
 *
 * @param void
 * @return void.
 */

void LETIMER0_IRQHandler(void)
{
	uint32_t Int_status = letimer_IntReset();

	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();

	if(Int_status == LETIMER_IF_COMP0)
	{
		timestamp_period_flag++;

		//if(BLE_connection_notification)
		{
			LETIMER_WAIT_FLAG_SET();
			gecko_external_signal(GECKO_LETIMER_WAIT_FLAG);
		}
	}

	else if(Int_status == LETIMER_IF_UF)
	{
		if(wait_flag)
		{
			timerSetEventInMsReturns();
		}
	}

	CORE_EXIT_CRITICAL();
	letimer_IntSet();
}

/**
 * @brief LETIMER initialization
 *
 * Function overview
 * Function sets the comparator values, performs sleep blocking,
 * Interrupt management and NVIC handling.
 *
 * @param void
 * @return void.
 */

void letimer_Init(void)
{
	LETIMER_Init(LETIMER0, &letimerInit);
	letimer_InterruptEnable();
}

/**
 * @brief LETIMER Interrupt Enable
 *
 * @param void
 * @return void.
 */

void letimer_InterruptEnable(void)
{
	letimer_Prescale();
	letimer_IntCompSet();
	letimer_IntSet();
	letimer_NvicEnable();
}

/**
 * @brief LETIMER Interrupt Disable
 *
 * @param void
 * @return void.
 */

void letimer_InterruptDisable(void)
{
	letimer_IntReset();
	letimer_NvicDisable();
}

/**
 * @brief LETIMER Interrupt Set
 *
 * @param void
 * @return void.
 */

void letimer_IntSet(void)
{
	uint32_t Int_flag;
	Int_flag = (LETIMER_IEN_COMP0 | LETIMER_IEN_UF);
	LETIMER_IntClear(LETIMER0, INT_CLEAR);
	LETIMER_IntEnable(LETIMER0, Int_flag);
}

/**
 * @brief LETIMER NVIC Set Enable
 *
 * @param void
 * @return void.
 */

void letimer_NvicEnable(void)
{
	NVIC_EnableIRQ(LETIMER0_IRQn);
}

/**
 * @brief LETIMER NVIC Set Disable
 *
 * @param void
 * @return void.
 */

void letimer_NvicDisable(void)
{
	NVIC_DisableIRQ(LETIMER0_IRQn);
}

/**
 * @brief LETIMER Comparator setting
 *
 * @param uint8_t power_mode
 * @return void.
 */

void letimer_IntCompSet(void)
{

	if(POWER_MODE <= 2)
	{
		COMP0_val = (uint32_t) (((PERIOD*LFXO_FREQ_Prescaled)/(MILLISECONDS)) - 1); // Period maintained by COMP0
	}

	else if(POWER_MODE == 3)
	{
		/* No prescaling required */
		COMP0_val = (uint32_t) (((PERIOD*ULFRCO_FREQ)/(MILLISECONDS)) - 1); // Period maintained by COMP0
	}

	LETIMER_CompareSet(LETIMER0, 0, COMP0_val);
}

/**
 * @brief LETIMER Interrupt Reset
 *
 * @param void
 * @return uint32_t Interrupt flag/status.
 */

uint32_t letimer_IntReset(void)
{
	uint32_t Int_status;
	Int_status = LETIMER_IntGetEnabled(LETIMER0);
	LETIMER_IntClear(LETIMER0, Int_status);
	LETIMER_IntDisable(LETIMER0, Int_status);
	return Int_status;
}

/**
 * @brief LETIMER sleep blocking setting
 *
 * @param void
 * @return void.
 */

void letimer_sleepblock(void)
{
	switch(POWER_MODE)
	{
		case 0:
				break;

		case 1: SLEEP_SleepBlockBegin(sleepEM2);
				break;

		case 2: SLEEP_SleepBlockBegin(sleepEM3);
				break;

		case 3:
				break;

		default:SLEEP_Sleep();
				break;
	}
}

/**
 * @brief LETIMER time stamp calculation function (in milli-Seconds)
 *
 * @param void
 * @return uint32_t.
 *
 * Comment: Added more timer roll-over safety.
 */

uint32_t letimer_TimeStampSet(void)
{
	uint32_t period_overtime;
	uint8_t extra_period = 0;
	if(timestamp_CNT_flag)
	{
		if(timestamp_CNT >= COMP1_val)
		{
			period_overtime = (timestamp_CNT - (COMP1_val - LETIMER0->CNT));
		}
		else
		{
			if((COMP1_val - LETIMER0->CNT) <= timestamp_CNT)
			{
				period_overtime = (timestamp_CNT - (COMP1_val - LETIMER0->CNT));
			}
			else
			{
				period_overtime = (COMP0_val - ((COMP1_val - LETIMER0->CNT) - timestamp_CNT));
				extra_period++;
			}
		}
	}
	else
	{
		period_overtime = LETIMER0->CNT;
	}

	uint32_t timestamp_val = ((timestamp_period_flag + extra_period - 1)*PERIOD) + ((uint32_t)((COMP0_val - period_overtime)*PERIOD)/(COMP0_val));

	return timestamp_val;
}

/**
 * @brief LETIMER Prescaling function
 *
 * @param void
 * @return void.
 */

void letimer_Prescale(void)
{
	if(POWER_MODE <= 2)
	{
		if(PERIOD <= 2000)
		{
			Prescale = 1;
		}
		else if(PERIOD <= 4000)
		{
			Prescale = 2;
		}
		else if(PERIOD <= 7000)
		{
			Prescale = 4;
		}

		LFXO_FREQ_Prescaled = (uint16_t) (LFXO_FREQ / Prescale);
	}
}

/**
 * @brief LETIMER Set event function with non-blocking non-inline waiting
 *
 * @param uint32_t ms_until_wakeup
 * @return void.
 */

void timerSetEventInMs(uint32_t ms_until_wakeup)
{
	wait_flag = 1;

	if(POWER_MODE <= 2)
	{
		float temp_val = (((float) (((float) (ms_until_wakeup)) / ((float) (MILLISECONDS))) * LFXO_FREQ_Prescaled) - 1);
		COMP1_val = (uint32_t) temp_val;
	}

	else if(POWER_MODE == 3)
	{
		/* No prescaling required */
		float temp_val = (((float) (((float) (ms_until_wakeup)) / ((float) (MILLISECONDS))) * ULFRCO_FREQ) - 1);
		COMP1_val = (uint32_t) temp_val;
	}

	LETIMER_CompareSet(LETIMER0, 1, COMP1_val);

	/* Capture current CNT value and restore it later after exiting blocking timer wait function */
	timestamp_CNT = LETIMER0->CNT;
	timestamp_CNT_flag = 1;
	LETIMER0->CNT = COMP1_val;

	/* Exiting blocking function and relying on event-driven state-machine */
}

/**
 * @brief LETIMER timer set event function clearing and resetting CNT/flags
 *
 * @param void
 * @return void.
 */

void timerSetEventInMsReturns(void)
{
	wait_flag = 0;

	if(timestamp_CNT >= COMP1_val)
	{
		LETIMER0->CNT = timestamp_CNT - COMP1_val;
	}
	else
	{
		LETIMER0->CNT = COMP0_val - (COMP1_val - timestamp_CNT);
	}

	timestamp_CNT_flag = 0;
}
