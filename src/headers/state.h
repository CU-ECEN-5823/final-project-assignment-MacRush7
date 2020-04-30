/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file state.h
 *
 * @brief State machine header file.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_STATE_H_
#define SRC_STATE_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include "header.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

/* Flag Macros */

/* Flag check succeeds if the flag is set. */
/* Flag set writes 1 to the flag bit position. */
/* Flag clear writes 0 to the flag bit position. */

#define LETIMER_WAIT_FLAG_SET()					(interrupt_event_bitmask |= 0x01)
#define LETIMER_WAIT_FLAG_CLEAR()				(interrupt_event_bitmask &= 0xFE)
#define LETIMER_WAIT_FLAG_CHECK()				({	\
													uint8_t flag; \
													flag = interrupt_event_bitmask & 0x01; \
													flag; \
												})

#define TEMP_I2C_WRITE_FLAG_SET()				(interrupt_event_bitmask |= 0x04)
#define TEMP_I2C_WRITE_FLAG_CLEAR()				(interrupt_event_bitmask &= 0xFB)
#define TEMP_I2C_WRITE_FLAG_CHECK()				({	\
													uint8_t flag; \
													flag = interrupt_event_bitmask & 0x04; \
													flag; \
												})

#define TEMP_I2C_READ_FLAG_SET()				(interrupt_event_bitmask |= 0x10)
#define TEMP_I2C_READ_FLAG_CLEAR()				(interrupt_event_bitmask &= 0xEF)
#define TEMP_I2C_READ_FLAG_CHECK()				({	\
													uint8_t flag; \
													flag = interrupt_event_bitmask & 0x10; \
													flag; \
												})

#define P_LETIMER_WAIT_FLAG_SET()				(event_processing_bitmask |= 0x01)
#define P_LETIMER_WAIT_FLAG_CLEAR()				(event_processing_bitmask &= 0xFE)
#define P_LETIMER_WAIT_FLAG_CHECK()				({	\
													uint8_t flag; \
													flag = event_processing_bitmask & 0x01; \
													flag; \
												})

#define P_TEMP_I2C_WRITE_FLAG_SET()				(event_processing_bitmask |= 0x04)
#define P_TEMP_I2C_WRITE_FLAG_CLEAR()			(event_processing_bitmask &= 0xFB)
#define P_TEMP_I2C_WRITE_FLAG_CHECK()			({	\
													uint8_t flag; \
													flag = event_processing_bitmask & 0x04; \
													flag; \
												})

#define P_TEMP_I2C_READ_FLAG_SET()				(event_processing_bitmask |= 0x10)
#define P_TEMP_I2C_READ_FLAG_CLEAR()			(event_processing_bitmask &= 0xEF)
#define P_TEMP_I2C_READ_FLAG_CHECK()			({	\
													uint8_t flag; \
													flag = event_processing_bitmask & 0x10; \
													flag; \
												})

/* Clearing unnecessary bits in the event_processing_bitmask variable. */

#define EVT_PR_BITM_CLEAR_EXTRA()				(event_processing_bitmask &= 0x1F)

/* State Machine External Event States */

#define GECKO_LETIMER_WAIT_FLAG				1
#define GECKO_TEMP_I2C_WRITE_FLAG			2
#define GECKO_TEMP_I2C_READ_FLAG			3

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

enum states
{
	/* State begins MCP9808 I2C write transaction. */
	STATE0_MCP9808_TRANSACTION_START_EVENT,

	/* State begins MCP9808 I2C read transaction. */
	STATE1_MCP9808_I2C_WRITE_COMPLETED_EVENT,

	/* State reports temperature readings. */
	STATE2_MCP9808_I2C_READ_COMPLETED_EVENT,

	/* Contains the number of states */
	NUM_STATES
};

/* Event flag variables */

volatile uint8_t event_processing_bitmask;
volatile uint8_t interrupt_event_bitmask;
extern uint8_t timerEnabled1HzSchedulerEvent;
extern volatile float temp_reading;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void sm_ClearInt(void);
void sm_HandleEvents(int current_state);
const char *getStateReport(int current_state);
void sm_Init(void);
void sm_HandleReset(int current_state);
void sm_ReportState(int current_state);

#endif /* SRC_STATE_H_ */
