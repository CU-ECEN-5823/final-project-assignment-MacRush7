/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */
#include <src/headers/gpio.h>

#define	LED0_port 			gpioPortF
#define LED0_pin			4
#define LED1_port 			gpioPortF
#define LED1_pin 			5

#define LCD_port			gpioPortD	// LCD display uses the same port as Si7021
#define LCD_pin				15			// LCD display uses the same pin as Si7021
#define LCD_EXTCOMIN_port	gpioPortD	// LCD display EXTCOMIN port
#define LCD_EXTCOMIN_pin	13			// LCD display EXTCOMIN pin

void gpioInit()
{
	/* LED GPIO configuration */
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	/* LCD GPIO configuration */
	GPIO_DriveStrengthSet(LCD_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(LCD_port, LCD_pin, gpioModePushPull, false);
	GPIO_DriveStrengthSet(LCD_EXTCOMIN_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_PinModeSet(LCD_EXTCOMIN_port, LCD_EXTCOMIN_pin, gpioModePushPull, false);
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}

void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}

void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}
void gpioEnableDisplay(void)
{
	GPIO_PinOutSet(LCD_port, LCD_pin);
}
void gpioSetDisplayExtcomin(bool high)
{
	if(high)
		GPIO_PinOutSet(LCD_EXTCOMIN_port, LCD_EXTCOMIN_pin);
	else
		GPIO_PinOutClear(LCD_EXTCOMIN_port, LCD_EXTCOMIN_pin);
}
