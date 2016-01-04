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
#include <bcm2835.h>
#include <cmdo.h>

/***************************************************************************
   Function:    
   Purpose:    
   Returns:
   Note:    
 **************************************************************************/
int main(int argc, char **argv)
{
	if (!bcm2835_init()) {
		return -1;
	}

	// set pin to be output
	bcm2835_gpio_fsel(LED_PIN,BCM2835_GPIO_FSEL_OUTP);

	while (1) {
		bcm2835_gpio_write(LED_PIN, HIGH);
		bcm2835_delay(500);
		bcm2835_gpio_write(LED_PIN, LOW);
		bcm2835_delay(500);
	}
	return 0;
}
