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
#include <avr/io.h>
#include <i2c.h>

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
	char hello[]="hello";

	i2c_init();


	i2c_transmit(0x50,hello,5);

	return 0;
}
