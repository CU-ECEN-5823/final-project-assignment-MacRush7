/***************************************************************************//**
 * @file
 * @brief Silicon Labs Bluetooth mesh light example
 * This example implements a Bluetooth mesh light node.
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

#include <src/headers/main.h>

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);

////////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************//**
 * Main function that handles all the external and BTM events and supports
 * BTM friend node capabilities.
 ******************************************************************************/

int main(void)
{
	// Initialize gecko mesh features
	gecko_mesh_init();

	// Initialize peripherals
	gecko_system_init();

	while (1)
	{
		gecko_external_evt_handler();

		struct gecko_cmd_packet *evt = gecko_wait_event();
		bool pass = mesh_bgapi_listener(evt);

		if (pass)
		{
			handle_ecen5823_gecko_event(BGLIB_MSG_ID(evt->header), evt);
		}
	}
}
