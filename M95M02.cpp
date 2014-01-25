/**************************************************************************/
/*!
 *  @file     M95M02.cpp
 *  @author   Clint Stevenson (Casco Logix)
 *  @license  
 *
 *  The M95M02 is a SPI-interface EEPROM chip with 2Mbit capacity
 *
 *  This is the source file for the M95M02 Arduino library
 *
 *  @section  HISTORY
 *  
 *  v1.0 - First release
 */
/**************************************************************************/


#include "Arduino.h"
#include <SPI.h>
#include "M95M02.h"


#define M95M02_CMD_WREN								0x06
#define M95M02_CMD_WRDI								0x04
#define M95M02_CMD_RDSR								0x05
#define M95M02_CMD_WRSR								0x01
#define M95M02_CMD_READ								0x03
#define M95M02_CMD_WRITE							0x02
#define M95M02_CMD_READ_ID							0x83
#define M95M02_CMD_WRITE_ID							0x82
#define M95M02_CMD_READ_LOCK_STATUS					0x83
#define M95M02_CMD_LOCK_ID							0x82

#define M95M02_STATUS_REGISTER_SRWD_BIT				0x80
#define M95M02_STATUS_REGISTER_BP1_BIT				0x08
#define M95M02_STATUS_REGISTER_BP0_BIT				0x04
#define M95M02_STATUS_REGISTER_BP_MASK		(M95M02_STATUS_REGISTER_BP1_BIT | \
											M95M02_STATUS_REGISTER_BP0_BIT)
#define M95M02_STATUS_REGISTER_WEL_BIT				0x02
#define M95M02_STATUS_REGISTER_WIP_BIT				0x01

#define M95M02_SEND_ADDRESS_STATE_DONE 				0
#define M95M02_SEND_ADDRESS_STATE_BUSY 				1


void commandWREN (void);
void commandWRITE (void);
void commandREAD (void);


M95M02::M95M02 ()
{

}


void M95M02::begin ()						// Initialize interfaces
{  
	SPI.begin();        					// Join I2C bus
}


uint8_t M95M02::write (uint32_t address, uint8_t data)			// Write a byte to EEPROM
{
	commandWRITE();						// Send WRITE command
	
    SPI.transfer((uint8_t)((address >> 16) && 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) && 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
	
    SPI.transfer(data);
	
	return 1;
}


uint8_t M95M02::write (uint32_t address, uint8_t * buffer, uint8_t numBytes) 
{
	commandWRITE();						// Send WRITE command
	
    SPI.transfer((uint8_t)((address >> 16) && 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) && 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
    
	uint8_t index;
    for (index = 0; index < numBytes; index++)
    {
		SPI.transfer(buffer[index]);
	}
	
	return index;
}
  
  
uint8_t M95M02::read (uint32_t address, uint8_t * buffer)	// Read a byte from EEPROM
{
	commandREAD();						// Send READ command

    SPI.transfer((uint8_t)((address >> 16) && 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) && 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
	
	*buffer = SPI.transfer(0);
	
    return 1;
}


uint8_t M95M02::read (uint32_t address, uint8_t * buffer, uint8_t numBytes)
{
	commandREAD();						// Send READ command

    SPI.transfer((uint8_t)((address >> 16) && 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) && 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
	
    uint8_t index;
    for (index = 0; index < numBytes; index++ )
	{
		buffer[index] = SPI.transfer(0);
	}
	
	return index;
}
  
  
uint8_t M95M02::getStatus (void)				// Read the EEPROM status 
{
	uint8_t retVal = 0;
	SPI.transfer(M95M02_CMD_RDSR);
	retVal = SPI.transfer(0);
	return retVal;
}


void M95M02::commandWREN (void)
{
    SPI.transfer(M95M02_CMD_WREN);
}


void commandWRITE (void)
{
    SPI.transfer(M95M02_CMD_WRITE);
}


void commandREAD (void)
{
    SPI.transfer(M95M02_CMD_READ);
}


uint8_t M95M02::getWIP (void)
{
	return (this->getStatus() & M95M02_STATUS_REGISTER_WIP_BIT);
}
