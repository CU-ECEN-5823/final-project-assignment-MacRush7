/*
 * ble_mesh_device_type.h
 *
 *  Created on: Mar 16, 2019
 *      Author: Dan Walkes
 */

#include <stdbool.h>

#ifndef SRC_HEADERS_BLE_MESH_DEVICE_TYPE_H_
#define SRC_HEADERS_BLE_MESH_DEVICE_TYPE_H_

/**
 * Set to 1 to build an level publisher client model for Assignment 10
 * Set to 0 to build an level subscriber server model for Assignment 10
 */
#define DEVICE_IS_Level_PUBLISHER			0

#if DEVICE_IS_Level_PUBLISHER
#define DEVICE_USES_BLE_MESH_CLIENT_MODEL 	1
#define DEVICE_USES_BLE_MESH_SERVER_MODEL 	0
#define DEVICE_IS_BLE_MESH_LPN 				1
#else
#define DEVICE_USES_BLE_MESH_CLIENT_MODEL 	0
#define DEVICE_USES_BLE_MESH_SERVER_MODEL 	1
#define DEVICE_IS_Level_SUBSCRIBER			1
#define DEVICE_IS_BLE_MESH_FRIEND 			1
#endif


#if DEVICE_IS_BLE_MESH_LPN
#define BUILD_INCLUDES_BLE_MESH_LPN 	1
static inline bool IsMeshLPN() { return true; }
static inline bool IsMeshFriend() { return false; }
#if DEVICE_IS_BLE_MESH_FRIEND
#error "Can't support both DEVICE_IS_BLE_MESH_LPN and DEVICE_IS_BLE_MESH_FRIEND set"
#endif
#endif

#if DEVICE_IS_BLE_MESH_FRIEND
#define BUILD_INCLUDES_BLE_MESH_LPN 	0
#define BUILD_INCLUDES_BLE_MESH_FRIEND 	1
static inline bool IsMeshLPN() { return false; }
static inline bool IsMeshFriend() { return true; }
#if DEVICE_IS_BLE_MESH_LPN
#error "Can't support both DEVICE_IS_BLE_MESH_LPN and DEVICE_IS_BLE_MESH_FRIEND set"
#endif
#endif


#if DEVICE_USES_BLE_MESH_CLIENT_MODEL
static inline bool DeviceUsesClientModel() { return true; }
#else
static inline bool DeviceUsesClientModel() { return false; }
#endif

#if DEVICE_USES_BLE_MESH_SERVER_MODEL
static inline bool DeviceUsesServerModel() { return true; }
#else
static inline bool DeviceUsesServerModel() { return false; }
#endif


#if DEVICE_IS_Level_PUBLISHER
static inline bool DeviceIsLevelPublisher() { return true; }
#else
static inline bool DeviceIsLevelPublisher() { return false; }
#endif

#if DEVICE_IS_Level_SUBSCRIBER
static inline bool DeviceIsLevelSubscriber() { return true; }
#else
static inline bool DeviceIsLevelSubscriber() { return false; }
#endif

#endif /* SRC_HEADERS_BLE_MESH_DEVICE_TYPE_H_ */
