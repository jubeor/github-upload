//#include "unity.h"
#include "unity_fixture.h"
#include "stdbool.h"
#include "stdio.h"

bool using_inverted_logic = true;
bool using_Inverted_Led_Index = true;

TEST_GROUP_RUNNER(LedDriver)
{
	for (int i=0; i<4; i++)
	{
		if(i%2)
			using_inverted_logic = !using_inverted_logic;
		if((i+1)%2)
			using_Inverted_Led_Index = !using_Inverted_Led_Index;

		printf("\n%d TESTs using inverted logic: %d\
				\nTESTs using inverted bits index: %d\
				\n-----------------------------\n",
				i,using_inverted_logic,using_Inverted_Led_Index);

		RUN_TEST_CASE(LedDriver, LedsOffAfterCreate);
		RUN_TEST_CASE(LedDriver, TurnOnLedOne);
		RUN_TEST_CASE(LedDriver, TurnOffLedOne);
		RUN_TEST_CASE(LedDriver, TurnOnMultipleLeds);
		RUN_TEST_CASE(LedDriver, TurnOffMultipleLeds);
		RUN_TEST_CASE(LedDriver, TurnOnAllLeds);
		RUN_TEST_CASE(LedDriver, TurnOffAllLeds);
		RUN_TEST_CASE(LedDriver, LedMemoryIsNotReadable);
		RUN_TEST_CASE(LedDriver, UpperAndLowerBounds);
		RUN_TEST_CASE(LedDriver, OutOfBoundsTurnOnDoesNoHarm);
		RUN_TEST_CASE(LedDriver, OutOfBoundsTurnOffDoesNoHarm);
		RUN_TEST_CASE(LedDriver, OutOfBoundsHandleError);
		RUN_TEST_CASE(LedDriver, QueryLedStateOn);
		RUN_TEST_CASE(LedDriver, QueryLedStateOff)
		RUN_TEST_CASE(LedDriver, QueryLedStateOutOfBoundsAlwaysOff);
	}
}
