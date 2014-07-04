/******************************************************************************/
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
 *  v1.1 - Bitbucket Issue #1:
 *		   Fixed data types in read and write to uint32_t to support full page
 *         read/writes as well as reading/writing the entire chip. 
 *		   Bitbucket Issue #2, Issue #3:
 *		   Added proposed code for accessing the EEPROM's ID page.
 */
/******************************************************************************/


#ifndef M95M02_H
#define M95M02_H


#include <inttypes.h>

#define M95M02_PAGE_SIZE			(256)
#define M95M02_NUM_PAGES			(1024)
#define M95M02_CAPACITY_BYTES		(M95M02_PAGE_SIZE * M95M02_NUM_PAGES)

class M95M02 {
public:
	M95M02(void);							// Constructor
	
	void begin(void);						// Initialize interfaces
	
	uint8_t write(uint32_t address, 		// Write a byte to EEPROM
					uint8_t data);
	uint8_t write(uint32_t address, 		// Write numBytes to EEPROM
					uint8_t * pBuffer, 
					uint32_t numBytes);
	uint8_t read(uint32_t address, 			// Read a byte from EEPROM
					uint8_t * pBuffer);
	uint8_t read(uint32_t address, 			// Read numBytes from EEPROM
					uint8_t * pBuffer, 
					uint32_t numBytes);
	uint8_t getStatus();					// Read the EEPROM status register
	void commandWREN();						// Set the Write Enable Latch (WEL) 
											// bit
	uint8_t getWIP();						// Get the Write In Progress (WIP) 
											// status flag from the status 
											// register
	void readIDPage (uint8_t address, 		// Reads the identification page to 
					uint8_t * pBuffer,		// a buffer.
					uint16_t size); 
	void writeIDPage (uint8_t address, 		// Writes the identification page 
					uint8_t * pBuffer, 		// from a buffer.
					uint16_t size);
	void readLockStatus (uint8_t * data);	// Reads the lock status from the 
											// EEPROM into a buffer
	void lockID (void);						// Sends the lock ID command
private:

};
#endif // M95M02_H