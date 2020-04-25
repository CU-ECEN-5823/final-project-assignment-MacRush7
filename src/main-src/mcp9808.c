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

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

#include <src/headers/mcp9808.h>

////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

void mcp9808_Write(void)
{
	I2C_Write(mcp9808_slave_addr, mcp9808_read_command);
}

void mcp9808_Read(void)
{
	I2C_Read(mcp9808_slave_addr);
}
