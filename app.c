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

/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Project Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file app.c
 *
 * @brief Application source C file.
 *
 * @author Rushi James Macwan
 */

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include "app.h"

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************//**
 * Handling of stack events. Both Bluetooth LE and Bluetooth mesh events
 * are handled here.
 * @param[in] evt_id  Incoming event ID.
 * @param[in] evt     Pointer to incoming event.
 ******************************************************************************/

void handle_ecen5823_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	if (NULL == evt)
	{
		return;
	}

	switch (evt_id)
	{
		/* BTM Gecko boot event. */
		case gecko_evt_system_boot_id:
		{
			/* Perform device reset that resets the LCD display and refreshes variables used by the BTM stack events. */
			gecko_device_reset();

			/* Check if the user requests for a device factory reset using the pushbuttons PB0/PB1. */
			if (GPIO_PinInGet(BSP_BUTTON0_PORT, BSP_BUTTON0_PIN) == 0 || GPIO_PinInGet(BSP_BUTTON1_PORT, BSP_BUTTON1_PIN) == 0)
			{
				/* Perform factory reset. */
				initiate_factory_reset();
			}

			else
			{
				displayPrintf(DISPLAY_ROW_CONNECTION, "Booting");

				/* Enabling pushbutton interrupts for clearing alarms and/or toggling display (on/off). */
				pushButton_EnableInt();

				/* Performing node initialisation for BTM. */
				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_init());
			}

			break;
		}

		/* BTM hardware software timer event. */
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
					/* Perform device (power) reset after factory reset is performed. */
					gecko_cmd_system_reset(0);
					break;
				}

				case TIMER_ID_FRIEND_FIND:
				{
					/* Attempt establishing friendship with LPN. */
					BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_establish_friendship(0));
					break;
				}

		        case TIMER_ID_RESTART:
		        {
		        	/* Perform device reset. */
		        	gecko_cmd_system_reset(0);
		        	break;
		        }

				default:
					break;
			}

			break;
  	  	}

  	  	/* BTM node initialised event. */
  	  	case gecko_evt_mesh_node_initialized_id:
  	  	{
  	  		displayPrintf(DISPLAY_ROW_CONNECTION, "Initialized");

  	  		struct gecko_msg_mesh_node_initialized_evt_t *pData = (struct gecko_msg_mesh_node_initialized_evt_t *)&(evt->data);

  	  		/* If node is provisioned, perform mesh initialisations. */
  	  		if(pData->provisioned)
  	  		{
  	  			displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");

  	  			/* Initialize mesh features. */
  	  			gecko_MeshInit();

  	  			/* Load alarms and print alarm/level statuses using persistent memory. */
  	  			gecko_load_alarms();
  	  			reset_print_alarm_buffer();
  	  		}

 	        else if(!evt->data.evt_mesh_node_initialized.provisioned)
 	        {
 	        	// enable ADV and GATT provisioning bearer
 	        	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_start_unprov_beaconing(0x3));
 	        }

  	  		break;
  	  	}

  	  	/* BTM provisioning started event. */
	    case gecko_evt_mesh_node_provisioning_started_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioning");

	    	break;
	    }

	    /* BTM provisioning successful event. */
	    case gecko_evt_mesh_node_provisioned_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioned");

			/* Initialize mesh features. */
			gecko_MeshInit();

			/* Load alarms and print alarm/level statuses using persistent memory. */
			gecko_load_alarms();
			reset_print_alarm_buffer();

	    	break;
	    }

	    /* BTM provisioning failed event. */
	    case gecko_evt_mesh_node_provisioning_failed_id:
	    {
	    	displayPrintf(DISPLAY_ROW_ACTION, "Provisioning Failed");

	    	/* start a one-shot timer that will trigger soft reset after small delay */
	    	gecko_cmd_hardware_set_soft_timer(1 * 32768, TIMER_ID_RESTART, 1);

	    	break;
	    }

	    /* BTM server client request acqusition event. */
        case gecko_evt_mesh_generic_server_client_request_id:
        {
        	mesh_lib_generic_server_event_handler(evt);

        	break;
        }

        /* BTM server state change event. */
        case gecko_evt_mesh_generic_server_state_changed_id:
        {
        	mesh_lib_generic_server_event_handler(evt);

        	break;
        }

        /* BTM connection opened event. */
	    case gecko_evt_le_connection_opened_id:
	    {
	    	displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");

	    	/* Maintaining the number of open connections. */
	    	num_connections++;
			conn_handle = evt->data.evt_le_connection_opened.connection;

			/* Update LCD display with the new number of active connections with the FN. */
			gecko_UpdateConnections();

			break;
	    }

	    /* BTM connection closed event. */
		case gecko_evt_le_connection_closed_id:
		{
			displayPrintf(DISPLAY_ROW_CONNECTION, "Disconnected");

			/* Check if need to boot to dfu mode */
			if (boot_to_dfu)
			{
				/* Enter to DFU OTA mode */
				gecko_cmd_system_reset(2);
			}

			/* Refresh the connection handle. */
			conn_handle = 0xFF;

			/* Refresh the number of active BTM connections with the FN. */
			num_connections--;

			/* Update LCD display with the new number of active connections with the FN. */
			gecko_UpdateConnections();

			break;
		}

		/* BTM node reset event. */
        case gecko_evt_mesh_node_reset_id:
        {
        	/* Clear display and perform factory reset. */
        	LCD_clearData();

        	/* Perform BTM device factory reset. */
        	initiate_factory_reset();

        	break;
        }

        /* BTM friendship established event. */
        case gecko_evt_mesh_lpn_friendship_established_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Friend EST.");
        	break;
        }

        /* BTM friendship failed event. */
        case gecko_evt_mesh_lpn_friendship_failed_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Friend FAILED");

        	/* Trigger timer to attempt establishing the friendship again. */
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000), TIMER_ID_FRIEND_FIND, 1);

			break;
        }

        /* BTM friendship termination event. */
        case gecko_evt_mesh_lpn_friendship_terminated_id:
        {
        	displayPrintf(DISPLAY_ROW_CONNECTION, "Friend TERM.");

        	/* Trigger timer to attempt establishing the friendship again. */
			gecko_cmd_hardware_set_soft_timer(TIMER_MS_2_TIMERTICK(2000), TIMER_ID_FRIEND_FIND, 1);

			break;
        }

        /* External signals event. */
		case gecko_evt_system_external_signal_id:
		{
			uint32_t ext_signal;
			ext_signal = evt->data.evt_system_external_signal.extsignals;

			/* If PB0 pushbutton is pressed, the FN would clear the alarm statuses and refresh the alarm buffer. */
			if(ext_signal == EXT_SIGNAL_PB0_PRESSED)
			{
				/* Clear alarm buffer */
				alarm_buffer = 0;
				gecko_store_alarms();
				reset_print_alarm_buffer();
			}

			/* If PB1 pushbutton is pressed, the FN would toggle the LCD display (turn on/off) based on its previous state. */
			if(ext_signal == EXT_SIGNAL_PB1_PRESSED)
			{
				static uint8_t LCD_flag = 1;

				if(LCD_flag)
				{
					LCD_flag = 0;
					gpioDisableDisplay();
				}

				else if(!LCD_flag)
				{
					LCD_flag = 1;
					gpioEnableDisplay();
					reset_print_alarm_buffer();
				}
			}

			break;
		}

		/* DFU write request event. */
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
