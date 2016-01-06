/***************************************************************************
 * spi_lcd
 *
 * SPI based lcd display
 **************************************************************************/
#include <cmdo.h>
#include <bcm2835.h>
#include <spi_lcd.h.h>

/**************************************************************************
    Function:	spi_lcd_init
    Purpose:	Initialize SPI device	
    Returns:	
    Note:	Sets defaults	
 *************************************************************************/
int spi_lcd_init(struct spi_lcd_dev *dev)
{
	// wait for 100ms for power up delay - could probably skip this.
	bcm2835_delay(100);

	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSeclect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0,LOW);
	
	return 0;
}

/**************************************************************************
    Function:	spi_lcd_close
    Purpose:	Returns GPIOs to normal state	
    Returns:	
    Note:	
 *************************************************************************/
int spi_lcd_close(struct spi_lcd_dev *dev)
{
	bcm2835_spi_end();
}


/**************************************************************************
    Function:	spi_lcd_off
    Purpose:	Turn off lcd display	
    Returns:	
    Note:	Assumes spi_lcd_init() has been called
 *************************************************************************/
void spi_lcd_off(void)
{
	u8 tx_buff[4] = { CMD_CODE, CMD_OFF, 0, 0 };
	u8 rx_buff[4];
	bcm2835_spi_transfernb(tx_buff, rx_buff, 2);
	bcm2835_delayMicroseconds(100);
}

	


