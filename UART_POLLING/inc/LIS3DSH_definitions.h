/*
 * LIS3DSH_definitions.h
 *
 *  Created on: Oct 25, 2017
 *      Author: jubeor
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LIS3DSH_DEFINITIONS_H_
#define LIS3DSH_DEFINITIONS_H_

/* Includes ------------------------------------------------------------------*/


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define LIS3DSH_READ_ADDRESS (uint8_t) 0b10000000 // to read we use a 1
#define LIS3DSH_WRITE_ADDRESS (uint8_t) 0b00000000 // to write we use a 0

#define OUT_T 		(uint8_t) 0x0C 	// Temperature register
#define INFO1 		(uint8_t) 0x0D 	// Info 1
#define INFO2		(uint8_t) 0x0E 	// Info 2
#define WHO_AM_I	(uint8_t) 0x0F 	// who am i OD
#define OFF_X		(uint8_t) 0x10	// X,Y,Z axis
#define OFF_Y		(uint8_t) 0x11
#define OUT_Z 		(uint8_t) 0x12
#define CS_X		(uint8_t) 0x13	// Constant Shift X
#define CS_Y		(uint8_t) 0x14	// Constant Shift X
#define CS_Z		(uint8_t) 0x15	// Constant Shift X
#define LC_L		(uint8_t) 0x16	// Long counter Low
#define LC_H		(uint8_t) 0x17	// Long counter high
#define STAT		(uint8_t) 0x18	// Interrupt synchronization
#define PEAK1		(uint8_t) 0x19	// Peak value
#define PEAK2		(uint8_t) 0x1A	// Peak value
// ...

#define OUTPUT_X_L		(uint8_t) 0x28	// Output X low
#define OUTPUT_X_H		(uint8_t) 0x29	// Output X high

#define OUTPUT_Y_L		(uint8_t) 0x2A	// Output Y low
#define OUTPUT_Y_H		(uint8_t) 0x2B	// Output Y high

#define OUTPUT_Z_L		(uint8_t) 0x2C	// Output Z low
#define OUTPUT_Z_H		(uint8_t) 0x2D	// Output Z high


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
