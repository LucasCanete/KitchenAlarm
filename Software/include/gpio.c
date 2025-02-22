#include "gpio.h"


void gpioSetMode(volatile uint8_t *port, uint8_t pin, uint8_t mode){
	
	switch(mode){
		case 0:
		//OUTPUT
		if(port == &DDRB){
			DDRB |= (1<<pin);
		}
		else if (port == &DDRC){
			DDRC |= (1<<pin);
		}
		else if (port == &DDRD){
			DDRD |= (1<<pin);
		}
		break;
		
		case 1:
		//INPUT
		if(port == &DDRB){
			DDRB &= ~(1<<pin);
		}
		else if (port == &DDRC){
			DDRC &= ~(1<<pin);
		}
		else if (port == &DDRD){
			DDRD &= ~(1<<pin);
		}
		break;
		
		case 2:
		//INPUT PULLUP
		if(port == &DDRB){
			DDRB &= ~(1<<pin);
			PORTB |= (1<<pin);
		}
		else if (port == &DDRC){
			DDRC &= ~(1<<pin);
			PORTC |= (1<<pin);
		}
		else if (port == &DDRD){
			DDRD &= ~(1<<pin);
			PORTD |= (1<<pin);
		}
		break;
		
	}
	

};

void gpioSetPin(volatile uint8_t *port, uint8_t pin, uint8_t state ){
	if(state == 1) *port |= (1<<pin);
	else if (state == 0) *port &= ~(1<<pin);
	
};

uint8_t gpioReadPin(volatile uint8_t *port, uint8_t pin){
	uint8_t data = 0;
	
	data = ((*port) & (1<<pin));

	
	return data;
};