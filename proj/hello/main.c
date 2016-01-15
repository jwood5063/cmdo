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
	char hello[]="hello";
	if (!bcm2835_init()) {
		return -1;
	}

	bcm2835_i2c_begin();
    	bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_03, BCM2835_GPIO_PUD_OFF); /* SDA */
    	bcm2835_gpio_set_pud(RPI_V2_GPIO_P1_05, BCM2835_GPIO_PUD_OFF); /* SCL */
	bcm2835_i2c_setSlaveAddress(0x50);
	bcm2835_i2c_set_baudrate(50000);
	bcm2835_delayMicroseconds(100);

	buffer[0]=CMD_CODE;
	buffer[1]=CMD_ON;
	bcm2835_i2c_write(buffer,2);
	bcm2835_delayMicroseconds(100);

	buffer[0]=CMD_CODE;
	buffer[1]=CMD_DISP_I2C_ADDR;
	bcm2835_i2c_write(buffer,2);
	bcm2835_delayMicroseconds(100);

	bcm2835_i2c_write(hello,strlen(hello));
		
	bcm2835_i2c_end();

	return 0;
}
