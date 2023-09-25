
#include "USART.h"




void initUSART(){
	//set Baudrate
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
	
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8 data bits, 1 stop bit */
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

//can transmit chars not integers
void transmitByte(uint8_t data){
	
	while(!(UCSR0A & (1<<UDRE0)));/* Wait for empty transmit buffer */
	UDR0 = data;/* send data */
}

uint8_t receiveByte(void){
	
	while(!(UCSR0A & (1<<RXC0)));/* Wait for data to be received */
	return UDR0;
}
void printString(const char myString []){
	uint8_t i = 0;
	while (myString[i]) {
	transmitByte(myString[i]);
	i++;
	}
}

void printBinaryByte(uint8_t byte){
	/* Prints out a byte as a series of 1's and 0's */
	uint8_t bit;
	for (bit=7; bit < 255; bit--){
		if ( bit_is_set(byte, bit) )
		transmitByte('1');
		else
		transmitByte('0');
	}
}

void printByte(uint8_t byte) {
	/* Converts a byte to a string of decimal text, sends it */
	transmitByte('0' + (byte / 100));                        /* Hundreds */
	transmitByte('0' + ((byte / 10) % 10));                      /* Tens */
	transmitByte('0' + (byte % 10));                             /* Ones */
}