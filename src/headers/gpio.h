/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_HEADERS_GPIO_H_
#define SRC_HEADERS_GPIO_H_

/* Header File */
#include "header.h"

#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	1
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		1

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioEnableDisplay(void);
void gpioSetDisplayExtcomin(bool high);

#endif /* SRC_HEADERS_GPIO_H_ */
