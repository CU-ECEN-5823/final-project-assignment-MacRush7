/*
 * display.h
 *
 *  Created on: Jan 1, 2019
 *      Author: Dan Walkes
 *
 * Use these steps to integrate the display module with your source code:
 * 1) Call displayInit with the content you want initialized on the header of the display.
 * 2) Add scheduler and timer events which can provide a 1Hz update for the display EXTCOMIN pin
 *  	through a call to displayUpdate().  Include your scheduler/timer header files in the top of
 *  	display.c.  #define these values in appropriate header files:
 *  	#define SCHEDULER_SUPPORTS_DISPLAY_UPDATE_EVENT 1
 *  	#define TIMER_SUPPORTS_1HZ_TIMER_EVENT	1
 * 3) Add functions gpioEnableDisplay() and gpioSetDisplayExtcomin(bool high) to your gpio.c and
 * 		gpio.h files, and include
 * 		#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	1
 * 		and
 *		#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		1
 *		in your gpio.h file
 */

#ifndef SRC_HEADERS_DISPLAY_H_
#define SRC_HEADERS_DISPLAY_H_

#define ECEN5823_INCLUDE_DISPLAY_SUPPORT 1

#include "glib.h"
#include "header.h"

enum display_row {
	DISPLAY_ROW_NAME,			// Device Type & Author Name
	DISPLAY_ROW_BTADDR,			// Device BT Address
	DISPLAY_ROW_CONNECTION,		// Device Connection Status
	DISPLAY_ROW_ACTION,			// Device Current Actions
	DISPLAY_ROW_LPN_MOISTURE,	// LPN - 1 (Moisture Sensor)
	DISPLAY_ROW_LPN_ALIGHT,		// LPN - 2 (Ambient Light Sensor)
	DISPLAY_ROW_LPN_UVLIGHT,	// LPN - 3 (UV Light Sensor)
	DISPLAY_ROW_TEMPERATURE,	// Local MCP9808 Temperature Sensor Reports
	DISPLAY_ROW_CONNECTIONS,	// Reports a record of existing LPN connections
	DISPLAY_ROW_MAX,			// Max number of rows display can support
};

uint8_t timerEnabled1HzSchedulerEvent;

#if ECEN5823_INCLUDE_DISPLAY_SUPPORT
void displayInit();
bool displayUpdate();
void displayPrintf(enum display_row row, const char *format, ... );
#else
static inline void displayInit() { }
static inline bool displayUpdate() { return true; }
static inline void displayPrintf(enum display_row row, const char *format, ... ) { row=row; format=format;}
#endif



#endif /* SRC_HEADERS_DISPLAY_H_ */
