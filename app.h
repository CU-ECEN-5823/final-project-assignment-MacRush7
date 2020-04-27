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

#include "src/headers/header.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************//**
 * \defgroup app Application Code
 * \brief Sample Application Implementation
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

/*******************************************************************************
 * GLOBAL VARIABLES & DECLARATIONS
 ******************************************************************************/

uint16_t BTM_ELEMENT_INDEX;
uint16_t BTM_ADDRESS;

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

/*******************************************************************************
 * Initialize used bgapi classes for server.
 ******************************************************************************/
void gecko_bgapi_classes_init_server_friend(void);

/*******************************************************************************
 * Initialize used bgapi classes for server.
 ******************************************************************************/
void gecko_bgapi_classes_init_client_lpn(void);

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
void pushButton_NodeInit(void);

static void PushButton_RequestHandler(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags);

static errorcode_t PushButton_PublishHandler(uint16_t element_index,
                                            uint32_t remaining_ms);

static void PushButton_ChangeHandler(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms);

void lpn_init(void);
void lpn_deinit(void);
void BTM_Reset(void);

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
};
#endif

#endif /* APP_H */
