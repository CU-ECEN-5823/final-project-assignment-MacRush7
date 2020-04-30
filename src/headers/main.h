/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file main.h
 *
 * @brief Main header file.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_HEADERS_MAIN_H_
#define SRC_HEADERS_MAIN_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include <src/headers/header.h>
#include <src/headers/ble_mesh_device_type.h>
#include <src/headers/gecko_mesh.h>
#include <src/headers/main_app.h>

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

/* Application code */
#include "app.h"

/* Main header file */
#include "src/headers/main.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#endif /* SRC_MAIN_H_ */
