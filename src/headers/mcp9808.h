/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Final Course Project Firmware
 * Node		: Friend Node
 * Author	: Rushi James Macwan
 ******************************************************************************/

/* @file mcp9808.h
 *
 * @brief MCP9808 High-accuracy Temperature Sensor (external) header file.
 * # Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/25095A.pdf
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_HEADERS_MCP9808_H_
#define SRC_HEADERS_MCP9808_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include "header.h"

////////////////////////////////////////////////////////////////////////////////
// DEFINE STATEMENTS
////////////////////////////////////////////////////////////////////////////////

/* MCP9808 slave address */
#define mcp9808_slave_addr		0x18

/* MCP9808 Command for reading temperature */
#define mcp9808_read_command	0x05

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void mcp9808_Write(void);
void mcp9808_Read(void);

#endif /* SRC_HEADERS_MCP9808_H_ */
