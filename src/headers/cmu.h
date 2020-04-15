/******************************************************************************
 * ECEN 5823 IoT Embedded Firmware (Spring-2020)
 * Assignment Submission
 * Author: Rushi James Macwan
 ******************************************************************************/

/* @file cmu.h
 *
 * @brief Clock management unit header file.
 *
 * @author Rushi James Macwan
 */

#ifndef SRC_HEADERS_CMU_H_
#define SRC_HEADERS_CMU_H_

////////////////////////////////////////////////////////////////////////////////
// HEADER FILES
////////////////////////////////////////////////////////////////////////////////

/* Header File */
#include "header.h"

////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPES
////////////////////////////////////////////////////////////////////////////////

void cmu_Init(void);
void cmu_ClkSelect(void);
void cmu_ClkEnable(void);

#endif /* SRC_HEADERS_CMU_H_ */
