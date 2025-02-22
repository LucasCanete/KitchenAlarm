/*
 * Buzzer.h
 *
 * Created: 15/8/2022 22:33:15
 *  Author: User
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdint.h>
#include <util/delay.h>

void buzzer_peep(volatile uint8_t *ddr_port, uint8_t port_number);
void buzzer_stop(volatile uint8_t *ddr_port, uint8_t port_number);


#endif /* BUZZER_H_ */