/*
 * Copyright (c) 2016 Jim Wood
 * All rights reserved.
 *
 *  file purpose
 *	LCD driver
 *	Uses SPI or I2C interface
 */

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////
#include <arcshell.h>

/**************************************************************************
    Function:	lcd_on
    Purpose:	Turn on lcd display	
    Returns:	
    Note:	Assumes spi_lcd_init() has been called
 *************************************************************************/
void lcd_on(struct lcd_dev *dev)
{
	u8 tx_buff[4] = { CMD_CODE, CMD_ON, 0, 0 };
	dev->write(dev,tx_buff, 2);
	bcm2835_delayMicroseconds(100);
}
/***************************************************************************
   Function:    
   Purpose:    
   Returns:
   Note:    
 **************************************************************************/
