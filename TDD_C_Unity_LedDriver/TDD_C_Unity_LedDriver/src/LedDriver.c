/*
 * LedDriver.c
 *
 *  Created on: Nov 10, 2018
 *      Author: boj
 */
#include "LedDriver.h"

static uint16_t * ledsAddress;
static uint16_t ledsImage;

enum {ALL_LEDS_ON = ~0, ALL_LEDS_OFF = ~ALL_LEDS_ON};

/* JBeltran Thinks:
 * an Inline function will not have it's own memory space, instead
 * it will be parsed by the compiler in the position where you call it.
 * with this approach or with a macro, we don't use the pile stack.
 *
 * From Learning C Language (Free ebook from stackoverflow)
 * For small functions that get called often, the overhead associated with
 * the function call can be a significant fraction of the total execution
 * time of that function. One way of improving performance, then, is to
 * eliminate the overhead.
 */
static inline uint16_t convertLedNumberToBit(int ledNumber)
{
	return 1 << (ledNumber-1);
}

static inline void updateHardware(void)
{
	*ledsAddress = ledsImage;
}

static inline uint8_t isLedOutOfBounds(int ledNumber)
{
	return (ledNumber<FIRST_LED || ledNumber>LAST_LED);
}

static inline void setLedImageBit(int ledNumber)
{
	ledsImage |= convertLedNumberToBit(ledNumber);
}

static inline void clearLedImageBit(int ledNumber)
{
	ledsImage &= ~convertLedNumberToBit(ledNumber);
}

void LedDriver_Create(uint16_t * address)
{
	ledsAddress = address;
	ledsImage = ALL_LEDS_OFF;
	updateHardware();
}

void LedDriver_Destroy(void)
{

}

void LedDriver_TurnOn(int ledNumber)
{
	if(isLedOutOfBounds(ledNumber))
		return; // space to handle the problem. Not implemented

	setLedImageBit(ledNumber);

	updateHardware();
}

void LedDriver_TurnOff(int ledNumber)
{
	if(isLedOutOfBounds(ledNumber))
		return; // space to handle the problem. Not implemented

	clearLedImageBit(ledNumber);

	updateHardware();
}

void LedDriver_TurnAllOn(void)
{
	ledsImage = ALL_LEDS_ON;

	updateHardware();
}

void LedDriver_TurnAllOff(void)
{
	ledsImage = ALL_LEDS_OFF;

	updateHardware();
}

bool LedDriver_isOn(int ledNumber)
{
	bool retVal = 0;
	// Here the test case is not detecting the out of bound problem
	if(isLedOutOfBounds(ledNumber))
	{
		retVal = false;
	}
	else
	{
		retVal = ledsImage & convertLedNumberToBit(ledNumber);
	}
	return retVal;
}

bool LedDriver_isOff(int ledNumber)
{
	return !LedDriver_isOn(ledNumber);;
}
