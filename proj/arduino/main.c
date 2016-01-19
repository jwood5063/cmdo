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
	char hello[]="hello";

	i2c_init();
	uart_init(115200);

	uart_puts(hello);
	uart_putc('\n');

	i2c_transmit(0x50,(u8 *)hello,5);

	return 0;
}
