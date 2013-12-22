

#ifndef M95M02_H
#define M95M02_H


#include <inttypes.h>


class M95M02 {
public:
	M95M02(uint8_t address);				// Constructor
	
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

private:

};
#endif // M95M02_H