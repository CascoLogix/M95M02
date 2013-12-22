
// by Clint Stevenson
// Casco Logix
// @CascoLogix
// http://www.cascologix.com
// 


#include <M95M02.h>


M95M02 spiEEPROM();      // Create instance of M95M02

void setup() 
{
  spiEEPROM.begin();      // Call begin to initialize instance
}

void loop() 
{
  delay(1000);                              // Wait one second
}