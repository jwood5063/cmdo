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
#define CMD_CODE		(0xfe)
#define CMD_ON			(0x41)
#define CMD_OFF			(0x42)
#define CMD_CUR_SET		(0x45)
#define CMD_CUR_HOME		(0x46)
#define CMD_CUR_UNDER_ON	(0x47)
#define CMD_CUR_UNDER_OFF	(0x48)
#define CMD_CUR_LEFT		(0x49)
#define CMD_CUR_RIGHT		(0x4a)
#define CMD_CUR_BLINK_ON	(0x4b)
#define CMD_CUR_BLINK_OFF	(0x4c)
#define CMD_BS			(0x4e)
#define CMD_CS			(0x51)
#define CMD_CONTRAST_SET	(0x52)
#define CMD_BRIGHTNESS_SET	(0x53)
#define CMD_LOAD_CHAR		(0x54)
#define CMD_DISP_LEFT		(0x55)
#define CMD_DISP_RIGHT		(0x56)
#define CMD_BAUD		(0x61)
#define CMD_I2C_ADDR		(0x62)
#define CMD_VERSION		(0x70)
#define CMD_DISP_BAUD		(0x71)
#define CMD_DISP_I2C_ADDR	(0x72)

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

