/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file header.h
 *
 * @brief Common header file containing all the required headers.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_HEADERS_HEADER_H_
#define SRC_HEADERS_HEADER_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Silicon Labs headers */
#include "gecko_configuration.h"
#include "native_gecko.h"
#include "em_core.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_device.h"
#include "em_gpio.h"
#include "em_letimer.h"
#include "retargetserial.h"
#include "sleep.h"
#include "gatt_db.h"
#include "infrastructure.h"
#include "glib.h"

/* Custom headers */
#include "cmu.h"
#include "gpio.h"
#include "letimer.h"
#include "log.h"
#include "display.h"
#include "gecko_ble_errors.h"
#include "ble_mesh_device_type.h"
#include "push_button.h"
#include "gpiointerrupt.h"
#include "mcp9808.h"
#include "i2c.h"
#include "state.h"

/* Standard headers */
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

/* To enable display support. */
#define DISPLAY_ENABLE

/* Enabled MCP9808 interface. */
#define MCP9808_ENABLED

/* Enable the LCD Display using the above #define statement. */
#ifdef DISPLAY_ENABLE
	#define SCHEDULER_SUPPORTS_DISPLAY_UPDATE_EVENT 1
	#define TIMER_SUPPORTS_1HZ_TIMER_EVENT			1
#endif

#endif /* SRC_HEADERS_HEADER_H_ */
