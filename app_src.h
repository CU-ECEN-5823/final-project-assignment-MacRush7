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

/* @file app_src.h
 *
 * @brief Application source header file.
 *
 * @author Rushi James Macwan
 */

#ifndef APP_SRC_H_
#define APP_SRC_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include "app.h"

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/// handle of the last opened LE connection
extern uint8_t conn_handle;

/// number of active Bluetooth connections
extern uint8_t num_connections;

/// Flag for indicating that initialization was performed
extern uint8_t boot_to_dfu;

/// Alarm buffers that keeps a record of the alarms since last provisioning.
extern uint8_t alarm_buffer;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void gecko_ecen5823_PrintDeviceAddress(void);
void initiate_factory_reset(void);
void set_device_name(bd_addr *pAddr);
void mesh_NodeInit(void);
void gecko_device_reset(void);
void LCD_clearData(void);
void gecko_load_alarms(void);
void gecko_store_alarms(void);
uint8_t mesh_friend_AlarmHandler(uint16_t client_addr, bool alarm);
void reset_print_alarm_buffer(void);
void gecko_UpdateConnections(void);
void gecko_MeshInit(void);

void Friend_RequestHandler	(uint16_t model_id,
                          	 uint16_t element_index,
							 uint16_t client_addr,
							 uint16_t server_addr,
							 uint16_t appkey_index,
							 const struct mesh_generic_request *request,
							 uint32_t transition_ms,
							 uint16_t delay_ms,
							 uint8_t request_flags);

void Friend_ChangeHandler	(uint16_t model_id,
                         	 uint16_t element_index,
							 const struct mesh_generic_state *current,
							 const struct mesh_generic_state *target,
							 uint32_t remaining_ms);

#ifdef __cplusplus
};
#endif

#endif /* APP_SRC_H_ */
