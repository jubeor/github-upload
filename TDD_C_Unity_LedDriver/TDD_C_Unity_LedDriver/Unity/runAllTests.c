/*
 * runAllTests.c
 *
 *  Created on: Nov 10, 2018
 *      Author: boj
 */


//#include "unity.h"
#include "unity_fixture.h"


static void RunAllTests(void)
{
	RUN_TEST_GROUP(LedDriver);
}


int main(int argc, const char * argv[])
{
	return UnityMain(argc, argv, RunAllTests);
}
