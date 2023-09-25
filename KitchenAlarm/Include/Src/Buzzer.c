/*
 * Buzzer.c
 *
 * Created: 15/8/2022 22:33:44
 *  Author: User
 */ 
#include <Buzzer.h>


void buzzer_peep(volatile uint8_t *ddr_port, uint8_t port_number){
	
	(*ddr_port) |= (1 << port_number);
	
};

void buzzer_stop(volatile uint8_t *ddr_port, uint8_t port_number){
	
	(*ddr_port) &= ~( 1 << port_number);
	
}