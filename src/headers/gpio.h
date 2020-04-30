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

#define	LED0_port 			gpioPortF
#define LED0_pin			4
#define LED1_port 			gpioPortF
#define LED1_pin 			5

#define LCD_port			gpioPortD	// LCD display uses the same port as Si7021
#define LCD_pin				15			// LCD display uses the same pin as Si7021
#define LCD_EXTCOMIN_port	gpioPortD	// LCD display EXTCOMIN port
#define LCD_EXTCOMIN_pin	13			// LCD display EXTCOMIN pin

void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void gpioDisableDisplay(void);
void gpioEnableDisplay(void);
void gpioSetDisplayExtcomin(bool high);

#endif /* SRC_HEADERS_GPIO_H_ */
