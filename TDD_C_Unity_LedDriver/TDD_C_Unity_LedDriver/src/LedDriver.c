/*
 * LedDriver.c
 *
 *  Created on: Nov 10, 2018
 *      Author: boj
 */
#include "LedDriver.h"

static uint16_t * ledsAddress;
static uint16_t ledsImage;
static bool usingInvertedLogic = false;
static bool usingInvertedLedIndex = false;

enum {
	ALL_LEDS_ON = ~0, ALL_LEDS_OFF = ~ALL_LEDS_ON
};

static inline uint16_t convertLedNumberToBit(int ledNumber) {
	return 1 << (ledNumber - 1);
}

static inline uint16_t ifNeededInvertLedIndex(uint16_t ledsImage) {
	uint16_t auxLedsImage = 0;
	uint16_t auxBitSelect = 0;
	uint16_t auxBitCopied = 0;
	int shift_right = 0;

	if (usingInvertedLedIndex) {
		for (int i = FIRST_LED - 1; i < LAST_LED; i++) {
			auxBitSelect = convertLedNumberToBit(LAST_LED - i);
			auxBitCopied = (ledsImage & auxBitSelect);
			shift_right = ((LAST_LED - 1 - i * 2));
			if (shift_right >= 0) {
				auxLedsImage |= (auxBitCopied) >> shift_right;
			} else {
				auxLedsImage |= (auxBitCopied) << -shift_right;
			}
			auxLedsImage |= (auxBitCopied) >> shift_right;
		}

	} else {
		auxLedsImage = ledsImage;
	}
	return auxLedsImage;
}

static inline uint16_t ifNeededInvertLogic(uint16_t ledsImage) {
	uint16_t retVal = 0;
	if (usingInvertedLogic) {
		retVal = ledsImage ^ 0xffff;
	} else {
		retVal = ledsImage;
	}
	return retVal;
}

static inline void updateHardware(void) {
	uint16_t auxledsImage = 0;

	auxledsImage = ifNeededInvertLedIndex(ledsImage);

	*ledsAddress = ifNeededInvertLogic(auxledsImage);
}

static inline uint8_t isLedOutOfBounds(int ledNumber) {
	return (ledNumber < FIRST_LED || ledNumber > LAST_LED);
}

static inline void setLedImageBit(int ledNumber) {
	ledsImage |= convertLedNumberToBit(ledNumber);
}

static inline void clearLedImageBit(int ledNumber) {
	ledsImage &= ~convertLedNumberToBit(ledNumber);
}

void LedDriver_Create(uint16_t * LedsRegister, bool useInvertedLogic,
		bool useInvertedLedIndex) {
	ledsAddress = LedsRegister;
	ledsImage = ALL_LEDS_OFF;
	usingInvertedLogic = useInvertedLogic;
	usingInvertedLedIndex = useInvertedLedIndex;
	updateHardware();
}

void LedDriver_Destroy(void) {
	usingInvertedLogic = false;
}

void LedDriver_TurnOn(int ledNumber) {
	if (isLedOutOfBounds(ledNumber))
		return; // space to handle the problem. Not implemented

	setLedImageBit(ledNumber);

	updateHardware();
}

void LedDriver_TurnOff(int ledNumber) {
	if (isLedOutOfBounds(ledNumber))
		return; // space to handle the problem. Not implemented

	clearLedImageBit(ledNumber);

	updateHardware();
}

void LedDriver_TurnAllOn(void) {
	ledsImage = ALL_LEDS_ON;

	updateHardware();
}

void LedDriver_TurnAllOff(void) {
	ledsImage = ALL_LEDS_OFF;

	updateHardware();
}

bool LedDriver_isOn(int ledNumber) {
	bool retVal = 0;
	// Here the test case is not detecting the out of bound problem
	if (isLedOutOfBounds(ledNumber)) {
		retVal = false;
	} else {
		retVal = ledsImage & convertLedNumberToBit(ledNumber);
	}
	return retVal;
}

bool LedDriver_isOff(int ledNumber) {
	return !LedDriver_isOn(ledNumber);;
}
