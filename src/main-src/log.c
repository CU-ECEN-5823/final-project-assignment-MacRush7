/*
 * log.c
 *
 *  Created on: Dec 18, 2018
 *      Author: Dan Walkes
 */

#include <src/headers/log.h>

#if INCLUDE_LOGGING
/**
 * @return a timestamp value for the logger, typically based on a free running timer.
 * This will be printed at the beginning of each log message.
 */
uint32_t loggerGetTimestamp(void)
{
	uint32_t timestamp_log = letimer_TimeStampSet();
	return timestamp_log;
}

/**
 * Initialize logging for Blue Gecko.
 * See https://www.silabs.com/community/wireless/bluetooth/forum.topic.html/how_to_do_uart_loggi-ByI
 */
void logInit(void)
{
	logFlush();

	RETARGET_SerialInit();
	/**
	 * See https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__RetargetIo.html#ga9e36c68713259dd181ef349430ba0096
	 * RETARGET_SerialCrLf() ensures each linefeed also includes carriage return.  Without it, the first character is shifted in TeraTerm
	 */
	RETARGET_SerialCrLf(true);
	LOG_INFO("Initialized Logging");
}

/*
 * Log temperature readings after successful I2C communication with Si7021
 */

void logTemp(void)
{
	logFlush();

	RETARGET_SerialInit();
	/**
	 * See https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__RetargetIo.html#ga9e36c68713259dd181ef349430ba0096
	 * RETARGET_SerialCrLf() ensures each linefeed also includes carriage return.  Without it, the first character is shifted in TeraTerm
	 */
	RETARGET_SerialCrLf(true);
	LOG_INFO("Temperature Reading (degree Celsius): %.3f C", temp_reading);
}

/*
 * Log error status for i2cConnect() function in i2c.h
 */

void logI2CWriteReturns(int status)
{
	logFlush();

	RETARGET_SerialInit();
	/**
	 * See https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__RetargetIo.html#ga9e36c68713259dd181ef349430ba0096
	 * RETARGET_SerialCrLf() ensures each linefeed also includes carriage return.  Without it, the first character is shifted in TeraTerm
	 */
	RETARGET_SerialCrLf(true);

	status += 5;

	const char *logStatus[] =
	{
		"i2cTransferSwFault",
		"i2cTransferUsageFault",
		"i2cTransferArbLost",
		"i2cTransferBusErr",
		"i2cTransferNack",
		"i2cTransferDone",
		"i2cTransferInProgress"
	};

	LOG_INFO("I2CSPM Write Function ERROR - Return status: %s", logStatus[status]);
}

/*
 * Log error status for i2cRead() function in i2c.h
 */

void logI2CReadReturns(int status)
{
	logFlush();

	RETARGET_SerialInit();
	/**
	 * See https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__RetargetIo.html#ga9e36c68713259dd181ef349430ba0096
	 * RETARGET_SerialCrLf() ensures each linefeed also includes carriage return.  Without it, the first character is shifted in TeraTerm
	 */
	RETARGET_SerialCrLf(true);

	status += 5;

	const char *logStatus[] =
	{
		"i2cTransferSwFault",
		"i2cTransferUsageFault",
		"i2cTransferArbLost",
		"i2cTransferBusErr",
		"i2cTransferNack",
		"i2cTransferDone",
		"i2cTransferInProgress"
	};

	LOG_INFO("I2CSPM Read Function ERROR - Return status: %s", logStatus[status]);
}

/*
 * Logging the state-machine status
 */

void logSM_Status(int current_state)
{
	logFlush();

	RETARGET_SerialInit();
	/**
	 * See https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__RetargetIo.html#ga9e36c68713259dd181ef349430ba0096
	 * RETARGET_SerialCrLf() ensures each linefeed also includes carriage return.  Without it, the first character is shifted in TeraTerm
	 */
	RETARGET_SerialCrLf(true);

	LOG_INFO("Current State: %s", getStateReport(current_state));
}

/*
 * Logging a user-specified string
 */

void logString(char* mystring)
{
	logFlush();

	RETARGET_SerialInit();
	/**
	 * See https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__RetargetIo.html#ga9e36c68713259dd181ef349430ba0096
	 * RETARGET_SerialCrLf() ensures each linefeed also includes carriage return.  Without it, the first character is shifted in TeraTerm
	 */
	RETARGET_SerialCrLf(true);
	LOG_INFO("%s", mystring);
}

/**
 * Block for chars to be flushed out of the serial port.  Important to do this before entering SLEEP() or you may see garbage chars output.
 */
void logFlush(void)
{
	RETARGET_SerialFlush();
}
#endif
