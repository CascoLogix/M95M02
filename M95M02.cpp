/******************************************************************************/
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
 *  v1.1 - Bitbucket Issue #1:
 *		   Fixed data types in read and write to uint32_t to support full page
 *         read/writes as well as reading/writing the entire chip. 
 *		   Bitbucket Issue #2, Issue #3:
 *		   Added proposed code for accessing the EEPROM's ID page.
 *  v1.2 - Bitbucket issue #4:
 *		   Fixed addressing bug where bitmask operation was using logical
 *         'AND' operator (&&) instead of bitwise 'AND' operator (&).
 */
/******************************************************************************/


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


/******************************************************************************/
/*!
 *  @function		M95M02
 *  @parameters		None
 *	@returns		None
 *	@description	This is the constructor for the M95M02 library.
/******************************************************************************/
M95M02::M95M02 (void)
{

}


/******************************************************************************/
/*!
 *  @function		begin
 *  @parameters		None
 *	@returns		None
 *	@description	Initializes the SPI interface on the Arduino using the call
					to the begin() method from the Arduino SPI library.
/******************************************************************************/
void M95M02::begin (void)
{  
	SPI.begin();
}


/******************************************************************************/
/*!
 *  @function		write
 *  @parameters		address, data
 *	@returns		number of bytes written (always 1)
 *	@description	Writes one byte to the specified address.
/******************************************************************************/
uint8_t M95M02::write (uint32_t address, uint8_t data)
{
	commandWRITE();										// Send WRITE command
	
    SPI.transfer((uint8_t)((address >> 16) & 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) & 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
	
    SPI.transfer(data);
	
	return 1;
}


/******************************************************************************/
/*!
 *  @function		write
 *  @parameters		address, pBuffer, numBytes
 *	@returns		number of bytes written
 *	@description	Writes the specified number of bytes starting at the 
 *					specified address. Data is written from a buffer which must
 *					be passed to the function. 
/******************************************************************************/
 uint8_t M95M02::write (uint32_t address, uint8_t * pBuffer, uint32_t numBytes) 
{
	commandWRITE();										// Send WRITE command
	
    SPI.transfer((uint8_t)((address >> 16) & 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) & 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
    
	uint32_t index;
    for (index = 0; index < numBytes; index++)
    {
		SPI.transfer(pBuffer[index]);
		// TODO: add loop exit to handle write errors
	}
	
	return index;
}
  

/******************************************************************************/
/*!
 *  @function		read
 *  @parameters		address, pBuffer
 *	@returns		number of bytes read (always 1)
 *	@description	Reads one byte from the specified address
/******************************************************************************/
uint8_t M95M02::read (uint32_t address, uint8_t * pBuffer)
{
	commandREAD();										// Send READ command

    SPI.transfer((uint8_t)((address >> 16) & 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) & 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
	
	*pBuffer = SPI.transfer(0);
	
    return 1;
}


/******************************************************************************/
/*!
 *  @function		read
 *  @parameters		address, pBuffer, numBytes
 *	@returns		number of bytes read
 *	@description	Reads the specified number of bytes starting from the 
 *					specified address. Data is placed into a buffer which must
 *					be passed to the function. Buffer size must be greater than
 *					or equal to the number of bytes being read.
/******************************************************************************/
uint8_t M95M02::read (uint32_t address, uint8_t * pBuffer, uint32_t numBytes)
{
	commandREAD();										// Send READ command

    SPI.transfer((uint8_t)((address >> 16) & 0xFF));	// MSB
    SPI.transfer((uint8_t)((address >> 8) & 0xFF));	
    SPI.transfer((uint8_t)(address & 0xFF)); 			// LSB
	
    uint32_t index;
    for (index = 0; index < numBytes; index++ )
	{
		pBuffer[index] = SPI.transfer(0);
	}
	
	return index;
}
  
  
/******************************************************************************/
/*!
 *  @function		getStatus
 *  @parameters		None
 *	@returns		EEPROM status register
 *	@description	Reads the status register from the EEPROM and returns the
 *					data
/******************************************************************************/
uint8_t M95M02::getStatus (void) 
{
	uint8_t retVal = 0;
	SPI.transfer(M95M02_CMD_RDSR);
	retVal = SPI.transfer(0);
	return retVal;
}


/******************************************************************************/
/*!
 *  @function		commandWREN
 *  @parameters		None
 *	@returns		None
 *	@description	Sends the WRITE_ENABLE command to the EEPROM. This should
 *					happen immediately before sending the write command, 
 *					including when writing to the EEPROM's status register.
/******************************************************************************/
void M95M02::commandWREN (void)
{
    SPI.transfer(M95M02_CMD_WREN);
}


/******************************************************************************/
/*!
 *  @function		commandWRITE
 *  @parameters		None
 *	@returns		None
 *	@description	Sends the WRITE command to the EEPROM. This should happen 
 *					immediately before sending data to be written, including 
 *					when writing to the EEPROM's status register.
/******************************************************************************/
void commandWRITE (void)
{
    SPI.transfer(M95M02_CMD_WRITE);
}


/******************************************************************************/
/*!
 *  @function		commandREAD
 *  @parameters		None
 *	@returns		None
 *	@description	Sends the READ command to the EEPROM. This should happen 
 *					immediately before reading data from the EEPROM.
/******************************************************************************/
void commandREAD (void)
{
    SPI.transfer(M95M02_CMD_READ);
}


/******************************************************************************/
/*!
 *  @function		getWIP
 *  @parameters		None
 *	@returns		WRITE_IN_PROGRESS flag from the EEPROM status register
 *	@description	Reads the EEPROM status register and returns the masked
 *					WIP flag. This function can be repeatedly called in a while
 *					loop to repeatedly poll the WIP flag until cleared for 
 *					faster write operation (as opposed to waiting in a delay
 *					loop for the worst-case write time).
/******************************************************************************/
uint8_t M95M02::getWIP (void)
{
	return (this->getStatus() & M95M02_STATUS_REGISTER_WIP_BIT);
}


/******************************************************************************/
/*!
 *  @function		readIDPage
 *  @parameters		address, pBuffer, size
 *	@returns		None
 *	@description	Reads the identification page to a buffer.
/******************************************************************************/
void M95M02::readIDPage (uint8_t address, uint8_t * pBuffer, uint16_t size)
{
    SPI.transfer(M95M02_CMD_READ_ID);
    SPI.transfer(0x00);			// MSB
    SPI.transfer(0x00);			// Address bit A10 must be 0; upper address 
								// bits are Don't Care
    SPI.transfer(address);		// LSB The data byte pointed to by the lower 
								// address bits
	
    uint16_t index;
    for(index = 0; index < size; index++)
	{
		pBuffer[index] = SPI.transfer(0);
	}
}


/******************************************************************************/
/*!
 *  @function		writeIDPage
 *  @parameters		address, pBuffer, size
 *	@returns		None
 *	@description	Writes the identification page from a buffer.
/******************************************************************************/
void M95M02::writeIDPage (uint8_t address, uint8_t * pBuffer, uint16_t size)
{
    SPI.transfer(M95M02_CMD_WRITE_ID);
    SPI.transfer(0x00);			// MSB
    SPI.transfer(0x00);			// Address bit A10 must be 0; upper address 
								// bits are Don't Care
    SPI.transfer(address);		// LSB The data byte pointed to by the lower 
								// address bits
	
    uint16_t index;
    for(index = 0; index < size; index++)
	{
    	SPI.transfer(pBuffer[index]);
	}
}


/******************************************************************************/
/*!
 *  @function		readLockStatus
 *  @parameters		data
 *	@returns		None
 *	@description	Reads the lock status from the EEPROM into a buffer
/******************************************************************************/
void M95M02::readLockStatus (uint8_t * data)
{
	SPI.transfer(M95M02_CMD_READ_LOCK_STATUS);
	SPI.transfer(0x00);			// MSB
	SPI.transfer(0x04);			// Address bit A10 must be 1; all other address 
								// bits are Don't Care
	SPI.transfer(0x00);			// LSB
	*data = SPI.transfer(0);
}


/******************************************************************************/
/*!
 *  @function		lockID
 *  @parameters		None
 *	@returns		None
 *	@description	Sends the lock ID command
/******************************************************************************/
void M95M02::lockID (void)
{
	SPI.transfer(M95M02_CMD_LOCK_ID);
	SPI.transfer(0x00);			// MSB
	SPI.transfer(0x04);			// Address bit A10 must be 1; all other address 
								// bits are Don't Care
	SPI.transfer(0x00);			// LSB
	SPI.transfer(0x02);			// The data byte sent must be equal to the 
								// binary value xxxx xx1x, where x = Don't Care
}