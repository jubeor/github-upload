/*
 * LedDriverTest.c
 *
 *  Created on: Nov 10, 2018
 *      Author: boj
 *
 * SUMMARY TEST LIST:
 *
 * 		ToDo: Clean the code. Get to work with Git effectively
 *
 *		Done: All leds are off after the driver is initialized
 *		Done: A single LED can be turned on
 *		Done: A single LED can be turned off
 *		Done: Multiple LEDs can be turned on
 *		Done: Multiple LEDs can be turned off
 *		Done: Turn on all LEDs
 *		Done: Turn off all LEDs
 *		Done: Check boundary values
 *		Done: Check write out of-bounds values
 *		Done: Query out of bounds values
 *		Done: Query if led is on
 *		Done: Query if led is off
 *		Done: inverted logic testing
 *		Done: inverted bit order testing
 *
 *
 *
 * Sorry about the "magic" numbers in the tests, I think they are quite ovious
 */


#include "unity_fixture.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "LedDriver.h"

TEST_GROUP(LedDriver);

#define MESSAGE_LENGHT 30

static uint16_t virtualLeds;

enum{ ALL_LEDS_ON = ~0,
	ALL_LEDS_OFF = ~ALL_LEDS_ON,
	FIRST_LED_ON = 0x0001,
	LAST_LED_ON = 0x8000};

enum{ OUT_OF_BOUNDS_POSITIVE_LED = 3141 ,
	OUT_OF_BOUNDS_NEGATIVE_LED = -1,
	IN_BOUND_LED_9 = 9,
	IN_BOUND_LED_11 = 11};

extern bool using_inverted_logic;
extern bool using_Inverted_Led_Index;


//helper function Begin

uint16_t correctLogicLevelVirtualLeds (uint16_t inputVirtualLeds)
{
	uint16_t TestOperations = 0;

	if(using_inverted_logic)
	{
		TestOperations = 0xffff;
	}
	return virtualLeds^TestOperations;
}

static inline uint16_t convertLedNumberToBit(int ledNumber)
{
	return 1 << (ledNumber-1);
}

uint16_t correctBitOrder(uint16_t virtualLeds)
{
	uint16_t auxLedsImage = 0;
	uint16_t auxBitSelect = 0;
	uint16_t auxBitCopied = 0;
	int shift_right = 0;

	if (using_Inverted_Led_Index)
	{
		for (int i=FIRST_LED-1; i< LAST_LED; i++ )
		{
			auxBitSelect = convertLedNumberToBit(LAST_LED-i);
			auxBitCopied = (virtualLeds & auxBitSelect);
			shift_right = ((LAST_LED-1-i*2));
			if (shift_right>=0)
			{
				auxLedsImage |= (auxBitCopied) >> shift_right;
			}
			else
			{
				auxLedsImage |= (auxBitCopied) << -shift_right;
			}
			auxLedsImage |= (auxBitCopied) >> shift_right;
		}

	}else
	{
		auxLedsImage = virtualLeds;
	}
	return auxLedsImage;
}

uint16_t compensatedVirtualLeds(void)
{
	uint16_t auxRegister = 	correctLogicLevelVirtualLeds (virtualLeds);
	return correctBitOrder(auxRegister);

}

void ledCanBeOn(int ledNumber)
{
	char * message = malloc(MESSAGE_LENGHT);
	sprintf(message,"error led number: %i",ledNumber);
	if (message == NULL){
		TEST_FAIL_MESSAGE("problem Allocating memory");
		return;
	}
	LedDriver_TurnOff(ledNumber);
	TEST_ASSERT_TRUE_MESSAGE(LedDriver_isOn(ledNumber),message);
	LedDriver_TurnOn(ledNumber);
	TEST_ASSERT_TRUE_MESSAGE(LedDriver_isOn(ledNumber),message);
	free(message);
}

void ledCanNotBeOn(int ledNumber)
{
	char * message = malloc(MESSAGE_LENGHT);
	if (message == NULL){
		TEST_FAIL_MESSAGE("problem Allocating memory");
		return;

	}
	sprintf(message,"error led number: %i \n",ledNumber);

	LedDriver_TurnOff(ledNumber);
	TEST_ASSERT_FALSE_MESSAGE(LedDriver_isOn(ledNumber),message);
	LedDriver_TurnOn(ledNumber);
	TEST_ASSERT_FALSE_MESSAGE(LedDriver_isOn(ledNumber),message);

	free(message);
}

//helper function End

TEST_SETUP(LedDriver)
{
	LedDriver_Create(&virtualLeds, using_inverted_logic, using_Inverted_Led_Index);
}

TEST_TEAR_DOWN(LedDriver)
{

}

TEST(LedDriver, LedsOffAfterCreate)
{
	virtualLeds = ALL_LEDS_ON;
	LedDriver_Create(&virtualLeds, using_inverted_logic, using_Inverted_Led_Index);

	TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF,compensatedVirtualLeds());
}

TEST(LedDriver, TurnOnLedOne)
{
	LedDriver_TurnOn(1);
	TEST_ASSERT_EQUAL_HEX16(FIRST_LED_ON, compensatedVirtualLeds());

}

TEST(LedDriver, TurnOffLedOne)
{
	LedDriver_TurnOn(1);
	LedDriver_TurnOff(1);
	TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, compensatedVirtualLeds());
}

TEST(LedDriver, TurnOnMultipleLeds)
{
	LedDriver_TurnOn(8);
	LedDriver_TurnOn(9);

	TEST_ASSERT_EQUAL_HEX16(0x0180, compensatedVirtualLeds());
}

TEST(LedDriver, TurnOffMultipleLeds)
{
	LedDriver_TurnAllOn();

	LedDriver_TurnOff(8);

	LedDriver_TurnOff(9);
	TEST_ASSERT_EQUAL_HEX16(~0x180 & 0xffff, compensatedVirtualLeds());
}

TEST(LedDriver, TurnOnAllLeds)
{
	LedDriver_TurnAllOn();

	TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_ON, compensatedVirtualLeds());
}

TEST(LedDriver, TurnOffAllLeds)
{
	LedDriver_TurnAllOn();

	LedDriver_TurnAllOff();

	TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_OFF, compensatedVirtualLeds());
}

TEST(LedDriver, LedMemoryIsNotReadable)
{
	virtualLeds = ALL_LEDS_ON;
	LedDriver_TurnOn(8);

	TEST_ASSERT_EQUAL_HEX16(0x0080, compensatedVirtualLeds());

}

TEST(LedDriver, UpperAndLowerBounds)
{
	LedDriver_TurnOn(FIRST_LED);
	LedDriver_TurnOn(LAST_LED);

	TEST_ASSERT_EQUAL_HEX16((LAST_LED_ON | FIRST_LED_ON), compensatedVirtualLeds());

}

TEST(LedDriver, OutOfBoundsTurnOnDoesNoHarm)
{
	LedDriver_TurnOn(OUT_OF_BOUNDS_NEGATIVE_LED);
	LedDriver_TurnOn(FIRST_LED-1);
	LedDriver_TurnOn(LAST_LED +1);
	LedDriver_TurnOn(OUT_OF_BOUNDS_POSITIVE_LED);

	TEST_ASSERT_EQUAL_HEX16(0, compensatedVirtualLeds());
}

TEST(LedDriver, OutOfBoundsTurnOffDoesNoHarm)
{
	LedDriver_TurnAllOn();
	LedDriver_TurnOff(OUT_OF_BOUNDS_NEGATIVE_LED);
	LedDriver_TurnOff(FIRST_LED-1);
	LedDriver_TurnOff(LAST_LED+1);
	LedDriver_TurnOff(OUT_OF_BOUNDS_POSITIVE_LED);

	TEST_ASSERT_EQUAL_HEX16(ALL_LEDS_ON, compensatedVirtualLeds());
}

IGNORE_TEST(LedDriver, OutOfBoundsHandleError)
{
	TEST_FAIL_MESSAGE("To be defined in a global strategy");
}

IGNORE_TEST(LedDriver, QueryLedStateOn)
{
	ledCanBeOn(FIRST_LED);
	ledCanBeOn(LAST_LED);
	ledCanBeOn(IN_BOUND_LED_11);
}

TEST(LedDriver, QueryLedStateOutOfBoundsAlwaysOff)
{
	ledCanNotBeOn(FIRST_LED-1);
	ledCanNotBeOn(LAST_LED+1);
	ledCanNotBeOn(OUT_OF_BOUNDS_NEGATIVE_LED);
	ledCanNotBeOn(OUT_OF_BOUNDS_POSITIVE_LED);
	for (int i = 1; i<OUT_OF_BOUNDS_POSITIVE_LED; i++)
	{
		ledCanNotBeOn(LAST_LED+i);
	}

}
TEST(LedDriver, QueryLedStateOff)
{
	LedDriver_TurnAllOn();
	TEST_ASSERT_FALSE(LedDriver_isOff(IN_BOUND_LED_9));
	LedDriver_TurnOff(IN_BOUND_LED_9);
	TEST_ASSERT_TRUE(LedDriver_isOff(IN_BOUND_LED_9));
}
