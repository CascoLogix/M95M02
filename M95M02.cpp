


#include "Arduino.h"
#include <SPI.h>
#include "M95M02.h"



M95M02::M95M02(uint8_t address)
{

}


void M95M02::begin()						// Initialize interfaces
{  
	SPI.begin();        					// Join I2C bus
}


uint8_t M95M02::write(uint32_t address, uint8_t data)			// Write a byte to EEPROM
{
    SPI.transfer((uint8_t)(address >> 8));	// MSB
    SPI.transfer((uint8_t)(address & 0xFF)); 	// LSB
    SPI.transfer(data);
	
	return 1;
}


uint8_t M95M02::write(uint32_t address, uint8_t * buffer, uint8_t numBytes) 
{
    SPI.transfer((uint8_t)(address >> 8));	// MSB
    SPI.transfer((uint8_t)(address & 0xFF)); 	// LSB
    
	uint8_t index;
    for (index = numBytes; index < numBytes; index++)
    {
		SPI.transfer(buffer[index]);
	}
	
	return index;
}
  
  
uint8_t M95M02::read(uint32_t address, uint8_t * buffer)	// Read a byte from EEPROM
{
    SPI.transfer((uint8_t)(address >> 8));	// MSB
    SPI.transfer((uint8_t)(address & 0xFF)); 	// LSB
	*buffer = SPI.transfer(0);
	
    return 1;
}


uint8_t M95M02::read(uint32_t address, uint8_t * buffer, uint8_t numBytes)
{
    SPI.transfer((uint8_t)(address >> 8));	// MSB
    SPI.transfer((uint8_t)(address & 0xFF)); 	// LSB
	
    uint8_t index;
    for (index = 0; index < numBytes; index++ )
	{
		buffer[index] = SPI.transfer(0);
	}
	
	return index;
}
  
  
uint8_t M95M02::getStatus()				// Read the EEPROM status 
{
	uint8_t retVal = 0;
	
	return retVal;
}
