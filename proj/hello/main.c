/* vim:ts=8:sw=8
 * Copyright (c) 2012 Quantenna Communications, Inc.
 * All rights reserved.
 *
 *  file purpose
 *
 */

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////
#define GLOBAL	// indicate globals declared in this file
#include <cmdo.h>
#include <bcm2835.h>
#include <lcd.h>

// hello world for i2c lcd display - no driver
/***************************************************************************
   Function:    
   Purpose:    
   Returns:
   Note:    
 **************************************************************************/
int main(int argc, char **argv)
{
	u8 buffer[32];
	if (!bcm2835_init()) {
		return -1;
	}

	bcm2835_i2c_begin();

	bcm2835_i2c_setSlaveAddress(0x50);
	bcm2835_delayMicroseconds(100);

	buffer[0]=CMD_CODE;
	buffer[1]=CMD_ON;
	bcm2835_i2c_write(buffer,2);
	bcm2835_delayMicroseconds(100);

	buffer[0]=CMD_CODE;
	buffer[1]=CMD_DISP_I2C_ADDR;
	bcm2835_i2c_write(buffer,2);
	bcm2835_delayMicroseconds(100);

	
	bcm2835_i2c_end();

	return 0;
}
