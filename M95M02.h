/**************************************************************************/
/*!
 *  @file     M95M02.h
 *  @author   Clint Stevenson (Casco Logix)
 *  @license  
 *
 *  The M95M02 is a SPI-interface EEPROM chip with 2Mbit capacity
 *
 *  This is the header file for the M95M02 Arduino library
 *
 *  @section  HISTORY
 *  
 *  v1.0 - First release
 */
/**************************************************************************/


#ifndef M95M02_H
#define M95M02_H


#include <inttypes.h>

#define M95M02_PAGE_SIZE			(256)
#define M95M02_NUM_PAGES			(1024)
#define M95M02_CAPACITY_BYTES		(M95M02_PAGE_SIZE * M95M02_NUM_PAGES)

class M95M02 {
public:
	M95M02();								// Constructor
	
	void begin();							// Initialize interfaces
	
	uint8_t write(uint32_t address, 		// Write a byte to EEPROM
					uint8_t data);
	uint8_t write(uint32_t address, 		// Write a byte to EEPROM
					uint8_t * buffer, 
					uint8_t numBytes);
	uint8_t read(uint32_t address, 			// Read a byte from EEPROM
					uint8_t * buffer);
	uint8_t read(uint32_t address, 			// Read a byte from EEPROM
					uint8_t * buffer, 
					uint8_t numBytes);
	uint8_t getStatus();					// Read the EEPROM status register
	void commandWREN();						// Set the Write Enable Latch (WEL) bit
	uint8_t getWIP();						// Get the Write In Progress (WIP) status flag from the status register
private:

};
#endif // M95M02_H