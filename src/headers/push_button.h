/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file push_button.h
 *
 * @brief On-board push-button handling header file.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_HEADERS_PUSH_BUTTON_H_
#define SRC_HEADERS_PUSH_BUTTON_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include "header.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

#define PB0_PORT			gpioPortF		// Push button 0 GPIO Port
#define PB0_PIN				6				// Push button 0 GPIO Pin

#define PB1_PORT			gpioPortF		// Push button 1 GPIO Port
#define PB1_PIN				7				// Push button 1 GPIO Pin

#define BUTTON_PRESSED		0x01			// button_state (service attribute)
#define BUTTON_RELEASED		0x00			// button_state (service attribute)

/*******************************************************************************
 * External signal definitions. These are used to signal button press events
 * from GPIO interrupt handler to application.
 ******************************************************************************/
#define EXT_SIGNAL_PB0_PRESSED		4
#define EXT_SIGNAL_PB0_RELEASED		5
#define EXT_SIGNAL_PB1_PRESSED		6
#define EXT_SIGNAL_PB1_RELEASED		7

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

volatile uint8_t PB0_val;
volatile uint8_t PB1_val;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void pushButton_Init(void);
void pushButton_EnableInt(void);
void pushButton_Reset(void);
void pushButton_IRQHandler(uint8_t pin);

#endif /* SRC_HEADERS_PUSH_BUTTON_H_ */
