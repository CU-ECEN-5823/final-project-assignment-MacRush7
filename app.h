/***************************************************************************//**
 * @file
 * @brief Application header file
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef APP_H
#define APP_H

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include "src/headers/header.h"

/* C Standard Library headers */
#include <stdio.h>

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "mesh_generic_model_capi_types.h"
#include "mesh_lib.h"

/* GPIO peripheral library */
#include <em_gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

#define TRUE 	1
#define FALSE 	0

/*******************************************************************************
 * Generic model IDs.
 ******************************************************************************/

/** Generic on/off server */
#define MESH_GENERIC_ON_OFF_SERVER_MODEL_ID       0x1000
/** Generic on/off client */
#define MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID       0x1001
/** Generic level server */
#define MESH_GENERIC_LEVEL_SERVER_MODEL_ID        0x1002
/** Generic level client */
#define MESH_GENERIC_LEVEL_CLIENT_MODEL_ID        0x1003

/** Stop timer. */
#define TIMER_STOP 0

/*******************************************************************************
 * Timer handles defines.
 ******************************************************************************/
#define TIMER_ID_RESTART            78
#define TIMER_ID_FACTORY_RESET      77
#define TIMER_ID_PROVISIONING       66
#define TIMER_ID_RETRANS_ONOFF      10
#define TIMER_ID_RETRANS_LIGHTNESS  11
#define TIMER_ID_RETRANS_CTL        12
#define TIMER_ID_RETRANS_SCENE      13
#define TIMER_ID_FRIEND_FIND        20
#define TIMER_ID_NODE_CONFIGURED    30
#define TIMER_ID_LCD_UPDATE			99

/*******************************************************************************
 * LPN addresses/indexes.
 ******************************************************************************/
/* Address 0x01 will be given to the FN. */

#define LPN_MOISTURE_ADDR			0x02
#define LPN_ALIGHT_ADDR				0x03
#define LPN_UVLIGHT_ADDR			0x04

#define LPN_MOISTURE_SET_INDEX		0b001
#define LPN_ALIGHT_SET_INDEX		0b010
#define LPN_UVLIGHT_SET_INDEX		0b100

#define LPN_MOISTURE_CLEAR_INDEX	0b110
#define LPN_ALIGHT_CLEAR_INDEX		0b101
#define LPN_UVLIGHT_CLEAR_INDEX		0b011

/*******************************************************************************
 * Alarm definitions.
 ******************************************************************************/
#define ALARM_SET		0xFFFF
#define ALARM_CLEARED	0x7FFF

/*******************************************************************************
 * Flash (Persistent Data) definitions.
 ******************************************************************************/
#define FLASH_ADDR			0x4000
#define FLASH_DATA_LENGTH	1
#define FLASH_OP_FAILED		0x0502

/*******************************************************************************
 * General defines.
 ******************************************************************************/

/// For indexing elements of the node (this example has only one element)
#define TIMER_CLK_FREQ ((uint32_t)32768)
/// Convert miliseconds to timer ticks
#define TIMER_MS_2_TIMERTICK(ms) ((TIMER_CLK_FREQ * ms) / 1000)

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

uint16_t BTM_ELEMENT_INDEX;
uint16_t BTM_ADDRESS;

uint8_t alarm_buffer;

struct PushButton_State
{
	uint8_t generic_onoff_current;
	uint8_t generic_onoff_target;
} BTM_PB_State;

struct Friend_States
{
	uint16_t moisture_level;
	uint16_t alight_level;
	uint16_t uvlight_level;
} Friend_Stat;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************//**
 * Handling of stack events. Both Bluetooth LE and Bluetooth mesh events
 * are handled here.
 * @param[in] evt_id  Incoming event ID.
 * @param[in] evt     Pointer to incoming event.
 ******************************************************************************/
void handle_ecen5823_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);

/*******************************************************************************
 * Other functions (some borrowed from SL BTM examples).
 ******************************************************************************/
void gecko_ecen5823_PrintDeviceAddress(void);
void initiate_factory_reset(void);
void set_device_name(bd_addr *pAddr);
void mesh_NodeInit(void);
void gecko_Device_Reset(void);
void LCD_clearData(void);
void gecko_load_alarm(void);
void gecko_store_alarm(void);
void pushButton_EnableInt(void);
uint8_t mesh_friend_AlarmHandler(uint16_t client_addr, bool alarm);

static void mesh_friend_level_request(uint16_t model_id,
                              uint16_t element_index,
                              uint16_t client_addr,
                              uint16_t server_addr,
                              uint16_t appkey_index,
                              const struct mesh_generic_request *request,
                              uint32_t transition_ms,
                              uint16_t delay_ms,
                              uint8_t request_flags);

static void mesh_friend_level_change(uint16_t model_id,
                             uint16_t element_index,
                             const struct mesh_generic_state *current,
                             const struct mesh_generic_state *target,
                             uint32_t remaining_ms);

#ifdef __cplusplus
};
#endif

#endif /* APP_H */
