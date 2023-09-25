#ifndef USART_H_
#define USART_H_
/*Created on 29/05/2022 Lucas Canete*/

#include <avr/io.h>

#define BAUD  9600
#include <util/setbaud.h>


void initUSART();

void transmitByte(uint8_t data);

uint8_t receiveByte(void);

void printString(const char myString []);
void printBinaryByte(uint8_t byte);
void printByte(uint8_t byte);
#endif