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

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "board_features.h"
#include "retargetserial.h"

/* Bluetooth stack headers */
#include "native_gecko.h"
#include "gatt_db.h"
#include <gecko_configuration.h>
#include <mesh_sizes.h>

/* Libraries containing default Gecko configuration values */
#include <em_gpio.h>

/* Coex header */
#include "coexistence-ble.h"

/* Device initialization header */
#include "hal-config.h"

/* Application config code */
#include "app_config.h"

/* Application code */
#include "app.h"

/* Main header file */
#include "src/headers/main.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES & DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);

/// Maximum number of simultaneous Bluetooth connections
#define MAX_CONNECTIONS 3

/// Heap for Bluetooth stack
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS) + BTMESH_HEAP_SIZE + 1760];

/// Bluetooth advertisement set configuration
///
/// At minimum the following is required:
/// * One advertisement set for Bluetooth LE stack (handle number 0)
/// * One advertisement set for Mesh data (handle number 1)
/// * One advertisement set for Mesh unprovisioned beacons (handle number 2)
/// * One advertisement set for Mesh unprovisioned URI (handle number 3)
/// * N advertisement sets for Mesh GATT service advertisements
/// (one for each network key, handle numbers 4 .. N+3)
///
#define MAX_ADVERTISERS (4 + MESH_CFG_MAX_NETKEYS)

/// Priorities for bluetooth link layer operations
static gecko_bluetooth_ll_priorities linklayer_priorities = GECKO_BLUETOOTH_PRIORITIES_DEFAULT;

/// Bluetooth stack configuration
const gecko_configuration_t config =
{
  .bluetooth.max_connections = MAX_CONNECTIONS,
  .bluetooth.max_advertisers = MAX_ADVERTISERS,
  .bluetooth.heap = bluetooth_stack_heap,
  .bluetooth.heap_size = sizeof(bluetooth_stack_heap) - BTMESH_HEAP_SIZE,
#if defined(FEATURE_LFXO)
  .sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
  .bluetooth.sleep_clock_accuracy = 100, // ppm
#elif defined(PLFRCO_PRESENT) || defined(LFRCO_PRESENT)
  .bluetooth.sleep_clock_accuracy = 500, // ppm
#endif
  .bluetooth.linklayer_priorities = &linklayer_priorities,
  .gattdb = &bg_gattdb_data,
  .btmesh_heap_size = BTMESH_HEAP_SIZE,
  .pa.config_enable = 1, // Set this to be a valid PA config
#if defined(FEATURE_PA_INPUT_FROM_VBAT)
  .pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#else
  .pa.input = GECKO_RADIO_PA_INPUT_DCDC,
#endif // defined(FEATURE_PA_INPUT_FROM_VBAT)
  .max_timers = 16,
  .rf.flags = GECKO_RF_CONFIG_ANTENNA,   // Enable antenna configuration.
  .rf.antenna = GECKO_RF_ANTENNA,   // Select antenna path!
};

////////////////////////////////////////////////////////////////////////////////
// MAIN FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	// Initialize device
	initMcu();
	// Initialize board
	initBoard();
	// Initialize application
	initApp();
	initVcomEnable();

	// Initialize peripherals
	gecko_system_init();

	// Minimize advertisement latency by allowing the advertiser to always
	// interrupt the scanner.
	linklayer_priorities.scan_max = linklayer_priorities.adv_min + 1;

	gecko_stack_init(&config);

	// Initialize the bgapi classes
	if( DeviceUsesClientModel() )
		gecko_bgapi_classes_init_client_lpn();

	else
		gecko_bgapi_classes_init_server_friend();

	// Initialize coexistence interface. Parameters are taken from HAL config.
	gecko_initCoexHAL();

	while (1)
	{
		#ifdef MCP9808_ENABLED
		{
			CORE_DECLARE_IRQ_STATE;
			CORE_ENTER_CRITICAL();
			{
			  event_processing_bitmask |= interrupt_event_bitmask;
			  EVT_PR_BITM_CLEAR_EXTRA();
			  sm_ClearInt();
			}
			CORE_EXIT_CRITICAL();

			if(event_processing_bitmask)
			{
			  static int i = 0;
			  for(i = 0; event_processing_bitmask != 0; i++)
				  sm_HandleEvents(i);
			}
		}
		#endif

		struct gecko_cmd_packet *evt = gecko_wait_event();
		bool pass = mesh_bgapi_listener(evt);

		if (pass)
		{
			handle_ecen5823_gecko_event(BGLIB_MSG_ID(evt->header), evt);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

void gecko_system_init(void)
{
	/* Initializing Peripherals and Configurations */
	sm_Init();
	gpioInit();
	pushButton_Init();
	cmu_Init();
	letimer_Init();
	i2c_Init();
	logInit();
	displayInit();
}
