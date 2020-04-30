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

/* @file app_src.c
 *
 * @brief Application source C file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include "app_src.h"

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************//**
 * This function process the requests for the generic level model
 * on primary element.
 *
 * @param[in] model_id       Server model ID.
 * @param[in] element_index  Server model element index.
 * @param[in] client_addr    Address of the client model which sent the message.
 * @param[in] server_addr    Address the message was sent to.
 * @param[in] appkey_index   The application key index used in encrypting the request.
 * @param[in] request        Pointer to the request structure.
 * @param[in] transition_ms  Requested transition time (in milliseconds).
 * @param[in] delay_ms       Delay time (in milliseconds).
 * @param[in] request_flags  Message flags. Bitmask of the following:
 *                           - Bit 0: Nonrelayed. If nonzero indicates
 *                                    a response to a nonrelayed request.
 *                           - Bit 1: Response required. If nonzero client
 *                                    expects a response from the server.
 ******************************************************************************/

void Friend_RequestHandler(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags)
{
	uint16_t level;
	level = (uint16_t) (request->level);

	if(level == ALARM_SET)
		alarm_buffer = mesh_friend_AlarmHandler(client_addr, TRUE);

	else if(level == ALARM_CLEARED)
		alarm_buffer = mesh_friend_AlarmHandler(client_addr, FALSE);

	else
		gecko_store_alarms();

	switch(client_addr)
	{
		case LPN_MOISTURE_ADDR:
		{
			if(!(alarm_buffer & LPN_MOISTURE_SET_ALARM_FLAG))
			{
				if(level == ALARM_CLEARED)
					displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "MOT: ALARM CLEARED");
				else
					displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "MOT (%): %d", level);
			}
			else
			{
				displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "MOT: ALARM");
			}
			break;
		}

		case LPN_ALIGHT_ADDR:
		{
			if(!(alarm_buffer & LPN_ALIGHT_SET_ALARM_FLAG))
			{
				if(level == ALARM_CLEARED)
					displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "ALT: ALARM CLEARED");
				else
					displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "ALT (%): %d", level);
			}
			else
			{
				displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "ALT: ALARM");
			}
			break;
		}

		case LPN_UVLIGHT_ADDR:
		{
			if(!(alarm_buffer & LPN_UVLIGHT_SET_ALARM_FLAG))
			{
				if(level == ALARM_CLEARED)
					displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "UVLT: ALARM CLEARED");
				else
					displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "UVLT (%): %d", level);
			}
			else
			{
				displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "UVLT: ALARM");
			}
			break;
		}

		default:
			break;
	}
}

/***************************************************************************//**
 * This function prints the alarm buffer status after device power cycle.
 ******************************************************************************/

void reset_print_alarm_buffer(void)
{
	if(alarm_buffer & LPN_MOISTURE_SET_ALARM_FLAG)
		displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "MOT: ALARM");
	else
		displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "-");


	if(alarm_buffer & LPN_ALIGHT_SET_ALARM_FLAG)
		displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "ALT: ALARM");
	else
		displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "-");


	if(alarm_buffer & LPN_UVLIGHT_SET_ALARM_FLAG)
		displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "UVLT: ALARM");
	else
		displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "-");
}

/***************************************************************************//**
 * This function is a handler for generic level alarm statuses. It handles
 * the acquired alarms and updates persistent data and the LCD display.
 *
 * @param[in] client_addr    Address of the BTM LPN client.
 * @param[in] alarm  		 Boolean variable stating if alarm needs to
 * 							 be set or cleared.
 ******************************************************************************/

uint8_t mesh_friend_AlarmHandler(uint16_t client_addr, bool alarm)
{
	uint8_t temp_alarm_flag;
	temp_alarm_flag = alarm_buffer;

	switch(client_addr)
	{
		case LPN_MOISTURE_ADDR:
		{
			if(alarm)
				temp_alarm_flag |= LPN_MOISTURE_SET_ALARM_FLAG;
			else
				temp_alarm_flag &= LPN_MOISTURE_CLEAR_ALARM_FLAG;

			break;
		}

		case LPN_ALIGHT_ADDR:
		{
			if(alarm)
				temp_alarm_flag |= LPN_ALIGHT_SET_ALARM_FLAG;
			else
				temp_alarm_flag &= LPN_ALIGHT_CLEAR_ALARM_FLAG;

			break;
		}

		case LPN_UVLIGHT_ADDR:
		{
			if(alarm)
				temp_alarm_flag |= LPN_UVLIGHT_SET_ALARM_FLAG;
			else
				temp_alarm_flag &= LPN_UVLIGHT_CLEAR_ALARM_FLAG;

			break;
		}

		default:
			break;
	}

	gecko_store_alarms();
	return temp_alarm_flag;
}

/***************************************************************************//**
 * This function is a handler for generic level change event
 * on primary element.
 *
 * @param[in] model_id       Server model ID.
 * @param[in] element_index  Server model element index.
 * @param[in] current        Pointer to current state structure.
 * @param[in] target         Pointer to target state structure.
 * @param[in] remaining_ms   Time (in milliseconds) remaining before transition
 *                           from current state to target state is complete.
 ******************************************************************************/

void Friend_ChangeHandler(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms)
{
	/* Since we are not requesting for any change in the LPN configuration
	 * or model information, and we are just requesting for information,
	 * as a subscriber we are not doing anything in the change handler here.
	 */
}

/***************************************************************************//**
 * This function prints BTM device address.
 ******************************************************************************/

void gecko_ecen5823_PrintDeviceAddress(void)
{
	/* Create structure to read BT server address. */
	struct gecko_msg_system_get_bt_address_rsp_t* gecko_server_address;

	/* Store the server address in a local data structure for later printing the address. */
	gecko_server_address = gecko_cmd_system_get_bt_address();

	char gecko_server_address_buffer[30];

	/* Store each address byte in the print buffer. */
	snprintf(gecko_server_address_buffer, sizeof(gecko_server_address_buffer), "%2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X",
			gecko_server_address->address.addr[5], gecko_server_address->address.addr[4], gecko_server_address->address.addr[3],
			gecko_server_address->address.addr[2], gecko_server_address->address.addr[1], gecko_server_address->address.addr[0]);

	displayPrintf(DISPLAY_ROW_BTADDR, gecko_server_address_buffer);

	struct gecko_msg_system_get_bt_address_rsp_t *pAddr = gecko_cmd_system_get_bt_address();

	set_device_name(&pAddr->address);
}

/***************************************************************************//**
 * This function performs device factory reset by erasing flash memory
 * (persistent data).
 ******************************************************************************/

void initiate_factory_reset(void)
{
	LCD_clearData();

	displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "*************");
	displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "FACTORY RESET");
	displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "*************");

	/* if connection is open then close it before rebooting */
	if (conn_handle != 0xFF)
	{
		gecko_cmd_le_connection_close(conn_handle);
	}

	/* Perform flash memory erase for device factory reset by removing provisioning information. */
	BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_erase_all());

	// reboot after a small delay
	gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_FACTORY_RESET, 1);
}

/***************************************************************************//**
 * This function clears the LCD display.
 ******************************************************************************/

void LCD_clearData(void)
{
	displayPrintf(DISPLAY_ROW_CONNECTION, "");
	displayPrintf(DISPLAY_ROW_ACTION, "");
	displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "");
	displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "");
	displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "");
	displayPrintf(DISPLAY_ROW_TEMPERATURE, "");
	displayPrintf(DISPLAY_ROW_CONNECTIONS, "");
}

/***************************************************************************//**
 * This function prints BTM device address.
 *
 * @param[in] pAddr       Pointer to BTM address of the device.
 ******************************************************************************/

void set_device_name(bd_addr *pAddr)
{
	char name[20];

	// create unique device name using the last two bytes of the Bluetooth address
	sprintf(name, "Friend Node (Rushi) - %02x:%02x", pAddr->addr[1], pAddr->addr[0]);

	// write device name to the GATT database
	BTSTACK_CHECK_RESPONSE(gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name, 0, strlen(name), (uint8_t *)name));

	// Logging device name on terminal
	LOG_INFO("%s", name);
}

/***************************************************************************//**
 * This function initialises mesh features - server, friend, allocates
 * memory for running BTM models and registers the respective handlers.
 ******************************************************************************/

void gecko_MeshInit(void)
{
	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_generic_server_init());
	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_friend_init());

	mesh_lib_init(malloc, free, 9);
	mesh_lib_generic_server_register_handler(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID, 0, Friend_RequestHandler, Friend_ChangeHandler, NULL);
}

/***************************************************************************//**
 * This function stores persistent data to the flash memory.
 ******************************************************************************/

void gecko_store_alarms(void)
{
	/* Save persistent data (alarm buffer) to flash memory. */
	BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_save(FLASH_ADDR, FLASH_DATA_LENGTH, &alarm_buffer));
}

/***************************************************************************//**
 * This function loads persistent data from the flash memory.
 ******************************************************************************/

void gecko_load_alarms(void)
{
	/* Load persistent data (alarm buffer) from flash memory. */

    uint16_t result;

	struct gecko_msg_flash_ps_load_rsp_t* flash_result;
    flash_result = gecko_cmd_flash_ps_load(FLASH_ADDR);

    alarm_buffer = flash_result->value.data[0];

    result = flash_result->result;
    if(result)
	{
		if(result == FLASH_OP_FAILED)
		{
			alarm_buffer = 0;
			gecko_store_alarms();
			gecko_load_alarms();
		}
		else
		{
			LOG_ERROR("Flash operation failed.");
		}
	}
}

/***************************************************************************//**
 * This function updates the LCD with the number of active BTM connections
 * with the FN.
 ******************************************************************************/

void gecko_UpdateConnections(void)
{
	if(num_connections < 0)
		num_connections = 0;

	LOG_INFO("Number of Connections: %d", num_connections);
	displayPrintf(DISPLAY_ROW_CONNECTIONS, "Connections: %d", num_connections);
}

/***************************************************************************//**
 * This function performs device reset by refreshing variables and by
 * printing device name and BT address.
 ******************************************************************************/

void gecko_device_reset(void)
{
	conn_handle = 0xFF;
	num_connections = 0;
	boot_to_dfu = 0;
	LCD_clearData();

	displayPrintf(DISPLAY_ROW_NAME, "Friend Node (Rushi)");
	gecko_ecen5823_PrintDeviceAddress();

	if(timerEnabled1HzSchedulerEvent)
		gecko_cmd_hardware_set_soft_timer(32768, TIMER_ID_LCD_UPDATE, 0);
}
