/***************************************************************************//**
 * @file
 * @brief Application code
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

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include "app.h"

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/// handle of the last opened LE connection
static uint8_t conn_handle = 0xFF;
/// number of active Bluetooth connections
static uint8_t num_connections = 0;
/// Flag for indicating that initialization was performed
static uint8_t boot_to_dfu = 0;
/// Flag to check if initialization has already occurred
static uint8_t mesh_init_flag = 0;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

void handle_ecen5823_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	if (NULL == evt)
	{
		return;
	}

	switch (evt_id)
	{
		case gecko_evt_system_boot_id:
		{
			gecko_Device_Reset();
			gecko_load_alarm();

			#ifdef PROJECT_TITLES
				displayPrintf(DISPLAY_ROW_NAME, "Friend Node");
			#else
				displayPrintf(DISPLAY_ROW_NAME, "Subscriber");
			#endif

			gecko_ecen5823_PrintDeviceAddress();

			if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN) == 0 || GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN) == 0)
			{
				initiate_factory_reset();
			}
			else
			{
				displayPrintf(DISPLAY_ROW_CONNECTION, "Booting");

				// Initialize Mesh stack in Node operation mode, it will generate initialized event
				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_init());
			}

			break;
		}

  	  	case gecko_evt_hardware_soft_timer_id:
  	  	{
			switch (evt->data.evt_hardware_soft_timer.handle)
			{
				case TIMER_ID_LCD_UPDATE:
				{
					/* Updating the LCD display. */
					displayUpdate();
					break;
				}

				case TIMER_ID_FACTORY_RESET:
				{
					// reset the device to finish factory reset
					gecko_cmd_system_reset(0);
					break;
				}

				case TIMER_ID_FRIEND_FIND:
				{
					BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_establish_friendship(0));
					break;
				}

		        case TIMER_ID_RESTART:
		        {
		        	// restart timer expires, reset the device
		        	gecko_cmd_system_reset(0);
		        	break;
		        }

				default:
				{
					break;
				}
			}

			break;
  	  	}

  	  	case gecko_evt_mesh_node_initialized_id:
  	  	{
  	  		displayPrintf(DISPLAY_ROW_CONNECTION, "Initialized");

  	  		struct gecko_msg_mesh_node_initialized_evt_t *pData = (struct gecko_msg_mesh_node_initialized_evt_t *)&(evt->data);

  	  		/* Friend Node --- Subscriber --- Server Model */
  	  		if(pData->provisioned)
  	  		{
  	  			BTM_ADDRESS = pData->address;
  	  			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_generic_server_init());

  	  			pushButton_EnableInt();

				/* Initialize mesh node models and register handler. */
				if(!mesh_init_flag)
					mesh_NodeInit();

				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_friend_init());
  	  		}

 	        else if(!evt->data.evt_mesh_node_initialized.provisioned)
 	        {
 	        	gecko_cmd_mesh_node_start_unprov_beaconing(0x3);   // enable ADV and GATT provisioning bearer
 	        }

  	  		break;
  	  	}

	    case gecko_evt_mesh_node_provisioning_started_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioning");
	    	break;
	    }

	    case gecko_evt_mesh_node_provisioned_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");

  	  		BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_generic_server_init());

  	  		pushButton_EnableInt();

  	  		/* Initialize mesh node models and register handler. */
			if(!mesh_init_flag)
				mesh_NodeInit();

			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_friend_init());

			break;
	    }

	    case gecko_evt_mesh_node_provisioning_failed_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioning Failed");

	    	/* start a one-shot timer that will trigger soft reset after small delay */
	    	gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_RESTART, 1);

	    	break;
	    }

        case gecko_evt_mesh_generic_server_client_request_id:
        {
        	mesh_lib_generic_server_event_handler(evt);
        	break;
        }

        case gecko_evt_mesh_generic_server_state_changed_id:
        {
        	mesh_lib_generic_server_event_handler(evt);
        	break;
        }

	    case gecko_evt_le_connection_opened_id:
	    {
	    	displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");

	    	num_connections++;
			conn_handle = evt->data.evt_le_connection_opened.connection;

			break;
	    }

		case gecko_evt_le_connection_closed_id:
		{
			displayPrintf(DISPLAY_ROW_CONNECTION, "Disconnected");

			/* Check if need to boot to dfu mode */
			if (boot_to_dfu)
			{
				/* Enter to DFU OTA mode */
				gecko_cmd_system_reset(2);
			}

			conn_handle = 0xFF;

			break;
		}

        case gecko_evt_mesh_node_reset_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Resetting");
        	initiate_factory_reset();

        	break;
        }

        case gecko_evt_mesh_lpn_friendship_established_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Found LPN");
        	LOG_INFO("Friendship established.");

        	break;
        }

        case gecko_evt_mesh_lpn_friendship_failed_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Friendship Failed");
        	LOG_ERROR("Error - Friendship failed.");

        	// try again in 2 seconds
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000), TIMER_ID_FRIEND_FIND, 1);

			break;
        }

        case gecko_evt_mesh_lpn_friendship_terminated_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Friend Terminated");
        	LOG_INFO("Error - Friendship Terminated.");

        	// try again in 2 seconds
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000), TIMER_ID_FRIEND_FIND, 1);

			break;
        }

//        case gecko_evt_system_external_signal_id:
//        {
//        	uint32_t ext_signal;
//        	ext_signal = evt->data.evt_system_external_signal.extsignals;
//
//        	if(ext_signal == EXT_SIGNAL_PB0_PRESSED)
//        	{
//        		/* Clear alarm buffer */
//        		alarm_buffer = 0;
//        		gecko_store_alarm();
//        		LOG_INFO("Alarm buffer cleared.");
//        	}
//
//        	break;
//        }

		case gecko_evt_gatt_server_user_write_request_id:
		{
			if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control)
			{
				/* Set flag to enter to OTA mode */
				boot_to_dfu = 1;
				/* Send response to Write Request */
				gecko_cmd_gatt_server_send_user_write_response(
				  evt->data.evt_gatt_server_user_write_request.connection,
				  gattdb_ota_control,
				  bg_err_success);

				/* Close connection to enter to DFU OTA mode */
				gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
			}

			break;
		}
	}
}

static void mesh_friend_level_request(uint16_t model_id,
                              uint16_t element_index,
                              uint16_t client_addr,
                              uint16_t server_addr,
                              uint16_t appkey_index,
                              const struct mesh_generic_request *request,
                              uint32_t transition_ms,
                              uint16_t delay_ms,
                              uint8_t request_flags)
{
//	uint16_t level;
//	level = (uint16_t) (request->level);
//
//	if(level == ALARM_SET)
//		alarm_buffer = mesh_friend_AlarmHandler(client_addr, TRUE);
//	else if(level == ALARM_CLEARED)
//		alarm_buffer = mesh_friend_AlarmHandler(client_addr, FALSE);
//
//	switch(client_addr)
//	{
//		case LPN_MOISTURE_ADDR:
//			if(!(alarm_buffer & LPN_MOISTURE_SET_INDEX))
//				displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "MOT: %d", level);
//			else
//				displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "MOT: ALARM");
//			break;
//
//		case LPN_ALIGHT_ADDR:
//			if(!(alarm_buffer & LPN_ALIGHT_SET_INDEX))
//				displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "ALT: %d", level);
//			else
//				displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "ALT: ALARM");
//			break;
//
//		case LPN_UVLIGHT_ADDR:
//			if(!(alarm_buffer & LPN_UVLIGHT_SET_INDEX))
//				displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "UVLT: %d", (uint16_t) level/1000);
//			else
//				displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "UVLT: ALARM");
//			break;
//
//		default:
//			break;
//	}

	LOG_INFO("Change in data: %d", request->level);
	displayPrintf(DISPLAY_ROW_TEMPERATURE, "%d", request->level);
}

uint8_t mesh_friend_AlarmHandler(uint16_t client_addr, bool alarm)
{
	uint8_t temp_alarm_flag;
	temp_alarm_flag = alarm_buffer;

	switch(client_addr)
	{
		case LPN_MOISTURE_ADDR:
		{
			if(alarm)
				temp_alarm_flag |= LPN_MOISTURE_SET_INDEX;
			else
				temp_alarm_flag &= LPN_MOISTURE_CLEAR_INDEX;
			break;
		}

		case LPN_ALIGHT_ADDR:
		{
			if(alarm)
				temp_alarm_flag |= LPN_ALIGHT_SET_INDEX;
			else
				temp_alarm_flag |= LPN_ALIGHT_CLEAR_INDEX;
			break;
		}

		case LPN_UVLIGHT_ADDR:
		{
			if(alarm)
				temp_alarm_flag |= LPN_UVLIGHT_SET_INDEX;
			else
				temp_alarm_flag |= LPN_UVLIGHT_CLEAR_INDEX;
			break;
		}

		default:
			break;
	}

	gecko_store_alarm();
	return temp_alarm_flag;
}

static void mesh_friend_level_change(uint16_t model_id,
                             uint16_t element_index,
                             const struct mesh_generic_state *current,
                             const struct mesh_generic_state *target,
                             uint32_t remaining_ms)
{
	// Do nothing
}

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

void initiate_factory_reset(void)
{
	LCD_clearData();

	displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "*************");
	displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "FACTORY RESET");
	displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "*************");

  /* if connection is open then close it before rebooting */
  if (conn_handle != 0xFF) {
	  BTSTACK_CHECK_RESPONSE(gecko_cmd_le_connection_close(conn_handle));
  }

  /* perform a factory reset by erasing PS storage. This removes all the keys and other settings
     that have been configured for this node */
  BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_erase_all());
  // reboot after a small delay
  gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_FACTORY_RESET, 1);
}

void LCD_clearData(void)
{
	displayPrintf(DISPLAY_ROW_CONNECTION, "");
	displayPrintf(DISPLAY_ROW_ACTION, "");
	displayPrintf(DISPLAY_ROW_LPN_MOISTURE, "");
	displayPrintf(DISPLAY_ROW_LPN_ALIGHT, "");
	displayPrintf(DISPLAY_ROW_LPN_UVLIGHT, "");
	displayPrintf(DISPLAY_ROW_TEMPERATURE, "");
}

void set_device_name(bd_addr *pAddr)
{
  char name[20];

  #if DEVICE_IS_Level_PUBLISHER
  	  // create unique device name using the last two bytes of the Bluetooth address
  	  sprintf(name, "Rushi LPN %02x:%02x", pAddr->addr[1], pAddr->addr[0]);
  #else
  	  // create unique device name using the last two bytes of the Bluetooth address
  	  sprintf(name, "Rushi FN %02x:%02x", pAddr->addr[1], pAddr->addr[0]);
  #endif

  // write device name to the GATT database
  gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name, 0, strlen(name), (uint8_t *)name);

  LOG_INFO("%s", name);
}

void mesh_NodeInit(void)
{
	mesh_init_flag = 1;

	mesh_lib_init(malloc, free, 9);
	mesh_lib_generic_server_register_handler(MESH_GENERIC_LEVEL_SERVER_MODEL_ID, 0, mesh_friend_level_request, mesh_friend_level_change, NULL);
}

void gecko_Device_Reset(void)
{
	BTM_ELEMENT_INDEX = 0;
	BTM_ADDRESS	= 0;
	mesh_init_flag = 0;

	if(timerEnabled1HzSchedulerEvent)
		gecko_cmd_hardware_set_soft_timer(32768, TIMER_ID_LCD_UPDATE, 0);
}

void gecko_store_alarm(void)
{
	BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_save(FLASH_ADDR, FLASH_DATA_LENGTH, &alarm_buffer));
}

void gecko_load_alarm(void)
{
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
			gecko_store_alarm();
			gecko_load_alarm();
		}
		else
		{
			LOG_ERROR("Flash operation failed.");
		}
	}
}
