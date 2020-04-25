/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file letimer.h
 *
 * @brief Low energy timer LETIMER header file.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_LETIMER_H_
#define SRC_LETIMER_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include "header.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

/* MACRO statements for changing interrupt PERIOD, ONTIME(Duty-cycle) and POWER_MODE
 *
 * Please Note: The PERIOD and ONTIME MACRO definitions except the below max and
 * min values. The values are required to be entered in terms of milli-seconds.
 * Any other values added to the PERIOD and ONTIME MACRO statements that fall
 * outside the range (that was specified in the assignment) will result into an undesired
 * timing and LED blinking (or associated event occurrence).
 *
 * ----------------------------------EXPECTED RANGES--------------------------------------
 * 				MAX (ms)			MIN (ms)
 * PERIOD			7000				1000
 * ONTIME			1000				100
 */

#define PERIOD 1000 //(ms) /* Set interrupt period to 3 seconds for generating Si7021-event */
#define ONTIME 175  //(ms)

	/*
	 * Power Mode Table:
	 *
	 * 0 - EM0
	 * 1 - EM1
	 * 2 - EM2
	 * 3 - EM3
	 * 4 - EM4
	 *
	 * Undefined power modes have been considered as a default case where ULFRCO is used.
	 */

#define POWER_MODE 3

#define INT_CLEAR (uint32_t)0x1F
#define LFXO_FREQ 32768
#define ULFRCO_FREQ 1000
#define MILLISECONDS 1000
#define MICROSECONDS 1000000

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

uint8_t Prescale;
uint8_t timestamp_CNT_flag;

uint16_t LFXO_FREQ_Prescaled;
uint16_t timestamp_CNT;

uint32_t COMP0_val;
uint32_t COMP1_val;
uint32_t timestamp_period_flag;

volatile uint8_t wait_flag;
extern uint8_t BLE_connection_notification;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void letimer_Init(void);
void letimer_InterruptEnable(void);
void letimer_InterruptDisable(void);
void letimer_IntSet(void);
void letimer_NvicEnable(void);
void letimer_NvicDisable(void);
void letimer_IntCompSet(void);
void LETIMER0_IRQHandler(void);
uint32_t letimer_IntReset(void);
void letimer_sleepblock(void);
void letimer_Prescale(void);
uint32_t letimer_TimeStampSet(void);
void timerSetEventInMs(uint32_t ms_until_wakeup);
void timerSetEventInMsReturns(void);

#endif /* SRC_LETIMER_H_ */
