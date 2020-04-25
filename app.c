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

/* Own header */
#include "app.h"
#include "src/headers/header.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/// handle of the last opened LE connection
static uint8_t conn_handle = 0xFF;
/// Flag for indicating that lpn feature is active
static uint8_t lpn_active = 0;
/// number of active Bluetooth connections
static uint8_t num_connections = 0;
/// For indexing elements of the node (this example has only one element)
#define TIMER_CLK_FREQ ((uint32_t)32768)
/// Convert miliseconds to timer ticks
#define TIMER_MS_2_TIMERTICK(ms) ((TIMER_CLK_FREQ * ms) / 1000)
/// Flag for indicating that initialization was performed
static uint8_t boot_to_dfu = 0;

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

/*******************************************************************************
 * Initialize used bgapi classes for server.
 ******************************************************************************/
void gecko_bgapi_classes_init_server_friend(void)
{
  gecko_bgapi_class_dfu_init();
  gecko_bgapi_class_system_init();
  gecko_bgapi_class_le_gap_init();
  gecko_bgapi_class_le_connection_init();
  //gecko_bgapi_class_gatt_init();
  gecko_bgapi_class_gatt_server_init();
  gecko_bgapi_class_hardware_init();
  gecko_bgapi_class_flash_init();
  gecko_bgapi_class_test_init();
  //gecko_bgapi_class_sm_init();
  gecko_bgapi_class_mesh_node_init();
  //gecko_bgapi_class_mesh_prov_init();
  gecko_bgapi_class_mesh_proxy_init();
  gecko_bgapi_class_mesh_proxy_server_init();
  //gecko_bgapi_class_mesh_proxy_client_init();
  //gecko_bgapi_class_mesh_generic_client_init();
  gecko_bgapi_class_mesh_generic_server_init();
  //gecko_bgapi_class_mesh_vendor_model_init();
  //gecko_bgapi_class_mesh_health_client_init();
  //gecko_bgapi_class_mesh_health_server_init();
  //gecko_bgapi_class_mesh_test_init();
  //gecko_bgapi_class_mesh_lpn_init();
  gecko_bgapi_class_mesh_friend_init();
  gecko_bgapi_class_mesh_lc_server_init();
  gecko_bgapi_class_mesh_lc_setup_server_init();
  gecko_bgapi_class_mesh_scene_server_init();
  gecko_bgapi_class_mesh_scene_setup_server_init();
}


/*******************************************************************************
 * Initialize used bgapi classes for client.
 ******************************************************************************/
void gecko_bgapi_classes_init_client_lpn(void)
{
	gecko_bgapi_class_dfu_init();
	gecko_bgapi_class_system_init();
	gecko_bgapi_class_le_gap_init();
	gecko_bgapi_class_le_connection_init();
	//gecko_bgapi_class_gatt_init();
	gecko_bgapi_class_gatt_server_init();
	gecko_bgapi_class_hardware_init();
	gecko_bgapi_class_flash_init();
	gecko_bgapi_class_test_init();
	//gecko_bgapi_class_sm_init();
	gecko_bgapi_class_mesh_node_init();
	//gecko_bgapi_class_mesh_prov_init();
	gecko_bgapi_class_mesh_proxy_init();
	gecko_bgapi_class_mesh_proxy_server_init();
	//gecko_bgapi_class_mesh_proxy_client_init();
	gecko_bgapi_class_mesh_generic_client_init();
	//gecko_bgapi_class_mesh_generic_server_init();
	//gecko_bgapi_class_mesh_vendor_model_init();
	//gecko_bgapi_class_mesh_health_client_init();
	//gecko_bgapi_class_mesh_health_server_init();
	//gecko_bgapi_class_mesh_test_init();
	gecko_bgapi_class_mesh_lpn_init();
	//gecko_bgapi_class_mesh_friend_init();
	gecko_bgapi_class_mesh_scene_client_init();
}

void handle_ecen5823_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	uint16_t result;

	if (NULL == evt)
	{
		return;
	}

	switch (evt_id)
	{
		case gecko_evt_system_boot_id:
		{
			BTM_Reset();

			if(DeviceIsOnOffPublisher())
			{
				displayPrintf(DISPLAY_ROW_NAME, "Low Power Node");
				//displayPrintf(DISPLAY_ROW_NAME, "Publisher");
			}
			else
			{
				displayPrintf(DISPLAY_ROW_NAME, "Friend Node");
				//displayPrintf(DISPLAY_ROW_NAME, "Subscriber");
			}

			gecko_ecen5823_PrintDeviceAddress();

			// check pushbutton state at startup. If either PB0 or PB1 is held down then do factory reset
			if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN) == 0 || GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN) == 0)
			{
				initiate_factory_reset();
			}
			else
			{
				displayPrintf(DISPLAY_ROW_CONNECTION, "Booting");

				// Initialize Mesh stack in Node operation mode, it will generate initialized event
				gecko_cmd_mesh_node_init();
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
					result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;
					if(!result)
					{
						LOG_ERROR("ret.code 0x%x\r\n", result);
					}
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
  	  		displayPrintf(DISPLAY_ROW_CONNECTION, "");

  	  		struct gecko_msg_mesh_node_initialized_evt_t *pData = (struct gecko_msg_mesh_node_initialized_evt_t *)&(evt->data);


  	  		// Low-power Node
  	  		if(pData->provisioned && DeviceIsOnOffPublisher())
  	  		{
  	  			BTM_ADDRESS = pData->address;

  	  			displayPrintf(DISPLAY_ROW_ACTION, "Initialized");

  	  			gecko_cmd_mesh_generic_client_init();

				/* Enabling pushbutton interrupts to handle button_state BLE service. */
				pushButton_EnableInt();
				pushButton_NodeInit();

				// Initialize Low Power Node functionality
				lpn_init();
  	  		}

  	  		// Friend Node
  	  		else if(pData->provisioned && DeviceIsOnOffSubscriber())
  	  		{
  	  			BTM_ADDRESS = pData->address;

  	  			displayPrintf(DISPLAY_ROW_ACTION, "Initialized");

  	  			gecko_cmd_mesh_generic_server_init();

				/* Enabling pushbutton interrupts to handle button_state BLE service. */
				pushButton_EnableInt();
				pushButton_NodeInit();

				// Initialize Low Power Node functionality
				//lpn_init();
				gecko_cmd_mesh_friend_init();

  	  		}

 	        else if(!evt->data.evt_mesh_node_initialized.provisioned)
 	        {
 	        	gecko_cmd_mesh_node_start_unprov_beaconing(0x3);   // enable ADV and GATT provisioning bearer
 	        }

  	  		break;
  	  	}

	    case gecko_evt_mesh_node_provisioning_started_id:
	    {
	    	LOG_INFO("Provisioning");
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioning");
	    	break;
	    }

	    case gecko_evt_mesh_node_provisioned_id:
	    {
  	  		if(DeviceIsOnOffPublisher())
  	  		{
  	  			displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");
  	  			gecko_cmd_mesh_generic_client_init();
  	  		}

  	  		else if(DeviceIsOnOffSubscriber())
  	  		{
  	  			displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");
  	  			gecko_cmd_mesh_generic_server_init();
  	  		}


			/* Enabling pushbutton interrupts to handle button_state BLE service. */
			pushButton_EnableInt();
			pushButton_NodeInit();

			// Initialize Low Power Node functionality
			lpn_init();

			break;
	    }

	    case gecko_evt_mesh_node_provisioning_failed_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "FAILED Provisioning");

	    	/* start a one-shot timer that will trigger soft reset after small delay */
	    	gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_RESTART, 1);

	    	displayPrintf(DISPLAY_ROW_CONNECTION, "Resetting");

	    	break;
	    }

        case gecko_evt_mesh_generic_server_client_request_id:
        {
        	if(DeviceUsesServerModel())
        	{
        		mesh_lib_generic_server_event_handler(evt);
        	}

        	break;
        }

        case gecko_evt_mesh_generic_server_state_changed_id:
        {
        	if(DeviceUsesServerModel())
        	{
        		mesh_lib_generic_server_event_handler(evt);
        	}

        	break;
        }

	    case gecko_evt_le_connection_opened_id:
	    {
			num_connections++;
			conn_handle = evt->data.evt_le_connection_opened.connection;

			displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");

			// turn off lpn feature after GATT connection is opened
			lpn_deinit();
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
			if (num_connections > 0)
			{
				if (--num_connections == 0)
				{
					// initialize lpn when there is no active connection
					lpn_init();
				}
			}

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
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Found Friend");
        	break;
        }

        case gecko_evt_mesh_lpn_friendship_failed_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Finding Friend");
        	LOG_ERROR("Error - Could not establish friendship.");

        	// try again in 2 seconds
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000), TIMER_ID_FRIEND_FIND, 1);

			break;
        }

        case gecko_evt_mesh_lpn_friendship_terminated_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Disconnected Friend");
        	LOG_DEBUG("Friendship Terminated");

        	// try again in 2 seconds
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000), TIMER_ID_FRIEND_FIND, 1);

			break;
        }

        case gecko_evt_system_external_signal_id:
        {
    			uint8_t PB_STAT = 0;
    			uint16_t  ret = 0;
    			static uint8_t transaction_id= 0;
    			struct mesh_generic_request req;

    			if(DeviceUsesClientModel())
				{
					PB_STAT = !GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN);

					if(PB_STAT)
						displayPrintf(DISPLAY_ROW_ALERT_NODES, "PB0 Pressed");
					else
						displayPrintf(DISPLAY_ROW_ALERT_NODES, "PB0 Released");

					req.kind = mesh_generic_request_on_off;

					if(PB_STAT)
						req.on_off = MESH_GENERIC_ON_OFF_STATE_ON;

					else
						req.on_off = MESH_GENERIC_ON_OFF_STATE_OFF;

					ret = mesh_lib_generic_client_publish(MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID, BTM_ELEMENT_INDEX, transaction_id, &req, 0,0,0);

					if(ret)
					{
						LOG_INFO("\nSend Failed\n");
					}
					else
					{
						LOG_INFO("\nvalue sent successfully\n");
					}

					transaction_id++;
				}

        	break;
        }

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

/**
 * @brief GECKO_ECEN5823 Gecko BLE device address printing
 *
 * Function overview
 * Prints the BT device address bytes.
 *
 * @param void
 * @return void.
 */

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
	displayPrintf(DISPLAY_ROW_CONNECTION, "*****");
	displayPrintf(DISPLAY_ROW_ACTION, "FACTORY RESET");
	displayPrintf(DISPLAY_ROW_CONNECTIONS, "*****");

  /* if connection is open then close it before rebooting */
  if (conn_handle != 0xFF) {
    gecko_cmd_le_connection_close(conn_handle);
  }

  /* perform a factory reset by erasing PS storage. This removes all the keys and other settings
     that have been configured for this node */
  gecko_cmd_flash_ps_erase_all();
  // reboot after a small delay
  gecko_cmd_hardware_set_soft_timer(2 * 32768, TIMER_ID_FACTORY_RESET, 1);

  displayPrintf(DISPLAY_ROW_CONNECTION, "");
  displayPrintf(DISPLAY_ROW_CONNECTIONS, "");
}

void set_device_name(bd_addr *pAddr)
{
  char name[20];

  #if DEVICE_IS_ONOFF_PUBLISHER
  	  // create unique device name using the last two bytes of the Bluetooth address
  	  sprintf(name, "5823Pub %02x:%02x", pAddr->addr[1], pAddr->addr[0]);
  #else
  	  // create unique device name using the last two bytes of the Bluetooth address
  	  sprintf(name, "5823Sub %02x:%02x", pAddr->addr[1], pAddr->addr[0]);
  #endif

  // write device name to the GATT database
  gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name, 0, strlen(name), (uint8_t *)name);

  // show device name on the LCD
  //displayPrintf(DISPLAY_ROW_CLIENTADDR, name);
  LOG_INFO("%s", name);
}

void pushButton_NodeInit(void)
{
	if(DeviceIsOnOffPublisher())
	{
		mesh_lib_init(malloc, free, 8);
	}

	else if(DeviceIsOnOffSubscriber())
	{
		mesh_lib_init(malloc, free, 9);
		mesh_lib_generic_server_register_handler(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID, 0, PushButton_RequestHandler, PushButton_ChangeHandler, NULL);
		PushButton_PublishHandler(0, 0);
	}
}

static void PushButton_RequestHandler(uint16_t model_id,
                          uint16_t element_index,
                          uint16_t client_addr,
                          uint16_t server_addr,
                          uint16_t appkey_index,
                          const struct mesh_generic_request *request,
                          uint32_t transition_ms,
                          uint16_t delay_ms,
                          uint8_t request_flags)
{
	if (BTM_PB_State.generic_onoff_current == request->on_off)
	{
		LOG_DEBUG("\nRequest for current state received; no op\n");
	}
	else
	{
		if(request->on_off)
			displayPrintf(DISPLAY_ROW_ALERT_NODES,"1");
		else
			displayPrintf(DISPLAY_ROW_ALERT_NODES,"0");

		if (transition_ms == 0 && delay_ms == 0)
		{
			BTM_PB_State.generic_onoff_current = request->on_off;
			BTM_PB_State.generic_onoff_target  = request->on_off;
		}
	}

	PushButton_PublishHandler(element_index, 0);
}

static errorcode_t PushButton_PublishHandler(uint16_t element_index,
                                            uint32_t remaining_ms)
{
  errorcode_t ret;
  struct mesh_generic_state current, target;

	current.kind = mesh_generic_state_on_off;
	current.on_off.on = BTM_PB_State.generic_onoff_current;

	target.kind = mesh_generic_state_on_off;
	target.on_off.on = BTM_PB_State.generic_onoff_target;

	ret = mesh_lib_generic_server_update(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
		                                        element_index,
		                                        &current,
		                                        &target,
		                                        remaining_ms);

	if (ret == bg_err_success) {
    ret = mesh_lib_generic_server_publish(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,
                                        element_index,
                                        mesh_generic_state_on_off);
  }

  return ret;
}

static void PushButton_ChangeHandler(uint16_t model_id,
                         uint16_t element_index,
                         const struct mesh_generic_state *current,
                         const struct mesh_generic_state *target,
                         uint32_t remaining_ms)
{
	if (current->on_off.on != BTM_PB_State.generic_onoff_current)
	{
		LOG_DEBUG("\n Switch On-Off state changed %u to %u\n", BTM_PB_State.generic_onoff_current, current->on_off.on);
		BTM_PB_State.generic_onoff_current = current->on_off.on;
	}
	else
	{
		LOG_DEBUG("\nDummy onoff change - same state as before\n");
	}
}

void lpn_init(void)
{
	uint16_t result;

	// Initialize LPN functionality.
	result = gecko_cmd_mesh_lpn_init()->result;
	if (result)
	{
		LOG_ERROR("LPN init failed (0x%x)\r\n", result);
		return;
	}

	// Configure LPN Minimum friend queue length = 2
	result = gecko_cmd_mesh_lpn_config(mesh_lpn_queue_length, 2)->result;
	if (result)
	{
		LOG_ERROR("LPN queue configuration failed (0x%x)\r\n", result);
		return;
	}

	// Configure LPN Poll timeout = 5 seconds
	result = gecko_cmd_mesh_lpn_config(mesh_lpn_poll_timeout, 5 * 1000)->result;
	if (result)
	{
		LOG_ERROR("LPN Poll timeout configuration failed (0x%x)\r\n", result);
		return;
	}

	result = gecko_cmd_mesh_lpn_establish_friendship(0)->result;
	if (result != 0)
	{
		LOG_ERROR("ret.code 0x%x\r\n", result);
	}
}

void lpn_deinit(void)
{

  if (!lpn_active) {
    return; // lpn feature is currently inactive
  }

  // Cancel friend finding timer
  gecko_cmd_hardware_set_soft_timer(TIMER_STOP,
                                             TIMER_ID_FRIEND_FIND,
                                             1);

  // Terminate friendship if exist
  gecko_cmd_mesh_lpn_terminate_friendship();

  // turn off lpn feature
  gecko_cmd_mesh_lpn_deinit();
  lpn_active = 0;
}

void BTM_Reset(void)
{
	BTM_ELEMENT_INDEX 					= 0;
	BTM_ADDRESS							= 0;
	BTM_PB_State.generic_onoff_current 	= -1;
	BTM_PB_State.generic_onoff_target	= 0;

	displayPrintf(DISPLAY_ROW_CONNECTION, "");
	displayPrintf(DISPLAY_ROW_ACTION, "");

	if(timerEnabled1HzSchedulerEvent)
		gecko_cmd_hardware_set_soft_timer(32768, TIMER_ID_LCD_UPDATE, 0);
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
