#ifndef __SPI_LCD_H__
#define __SPI_LCD_H__
/*
 * Copyright (c) 2016 Jim Wood
 * All rights reserved.
 *
 *  file purpose
 *
 */

////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////
struct spi_lcd_dev {
	u32 addr;
};

////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////
/**************************************************************************
    Function:	spi_lcd_init
    Purpose:	Initialize SPI device	
    Returns:	
    Note:	Sets defaults	
 *************************************************************************/
extern int spi_lcd_init(struct spi_lcd_dev *dev);

/**************************************************************************
    Function:	spi_lcd_close
    Purpose:	Returns GPIOs to normal state	
    Returns:	
    Note:	
 *************************************************************************/
int spi_lcd_close(struct spi_lcd_dev *dev);

/**************************************************************************
    Function:	spi_lcd_on
    Purpose:	Turn on lcd display	
    Returns:	
    Note:	Assumes spi_lcd_init() has been called
 *************************************************************************/
void spi_lcd_on(void);

/**************************************************************************
    Function:	spi_lcd_off
    Purpose:	Turn off lcd display	
    Returns:	
    Note:	Assumes spi_lcd_init() has been called
 *************************************************************************/
void spi_lcd_off(void);
{

#endif // __SPI_LCD_H__

