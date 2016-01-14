/*
 * Copyright (c) 2016 Jim Wood
 * All rights reserved.
 *
 *  file purpose
 *
 */

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////
#include <cmdo.h>
#include <avr/io.h>
#include <avr/iom2560.h>
#include <util/delay.h>

int adc_init(void)
{
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
	// Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow 
				//	easy 8 bit reading

	// No MUX values needed to be changed to use ADC0

	//ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
	ADCSRB &= ~7;

	ADCSRA |= (1 << ADEN);  // Enable ADC
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions

	return 0;
}


int main (void)
{
	uint8_t		leds = 1;
	//uint32_t value;

	/* set PORTB for output*/
	DDRB = 0xFF;

	while(1) {
		for (leds = 1; leds <= (1 << 5); leds <<= 1) {
			PORTB = leds;
			_delay_ms(200);
		}
		for (leds = (1 << 4); leds >= 2; leds >>= 1) {
			PORTB = leds;
			_delay_ms(200);
		}
	}

	adc_init();

	if (ADCH > 0) {
	}

	return 0;
}
