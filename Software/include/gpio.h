#ifndef GPI0_H
#define GPIO_H

#include <avr/io.h>
#include <stdint.h>

/*Defining Macros*/
#define BV(bit)				(1 << (bit) )
#define setBit(byte,bit)		(byte |= BV(bit))
#define clearBit(byte,bit)		(byte &= ~BV(bit))

#define PB          1
#define PC          2
#define PD          3

#define MODE_OUTPUT		0
#define MODE_INPUT		1
#define MODE_INPUT_PULLUP	2

void gpioSetMode(volatile uint8_t *port, uint8_t pin, uint8_t mode);

void gpioSetPin(volatile uint8_t *port, uint8_t pin, uint8_t state);

uint8_t gpioReadPin(volatile uint8_t *port, uint8_t pin);


#endif




