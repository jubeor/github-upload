/*
 * LedDriver.h
 *
 *  Created on: Nov 10, 2018
 *      Author: boj
 */

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

#include "stdint.h"
#include "stdbool.h"

void LedDriver_Create(uint16_t * LedsRegister);
void LedDriver_Destroy(void);
void LedDriver_TurnOn(int ledNumber);
void LedDriver_TurnOff(int ledNumber);
void LedDriver_TurnAllOn(void);
void LedDriver_TurnAllOff(void);
bool LedDriver_isOn(int ledNumber);
bool LedDriver_isOff(int ledNumber);

enum {FIRST_LED = 1, LAST_LED = 16};

#endif /* LEDDRIVER_H_ */
