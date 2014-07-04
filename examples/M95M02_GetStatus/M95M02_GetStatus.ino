// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com
// 


#include <M95M02.h>
#include <SPI.h>


M95M02 SPI_EEPROM;      // Create instance of MCP23S08 with CS on pin 3

void setup() 
{
  Serial.begin(9600);
  SPI_EEPROM.begin();      // Call begin to initialize instance
  Serial.print(SPI_EEPROM.getStatus(), BIN);
  Serial.print("Done.");
}

void loop() 
{
  Serial.print(".");
  delay(1000);                              // Wait one second
}