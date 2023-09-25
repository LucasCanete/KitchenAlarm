/*
 * SevSeg.h
 *
 * Created: 7/8/2022 23:28:30
 *  Author: User
 */ 


#ifndef SEVSEG_H_
#define SEVSEG_H_

#include <stdint.h>

/*
extern int8_t ten_min;
extern int8_t unit_min;
extern int8_t ten_sec;
extern int8_t unit_sec;
*/
//{A,B,C,D,E,F,G}
/*	
uint8_t segment[10][7] = {
	{0,0,0,0,0,0,1}, //0
	{1,0,0,1,1,1,1}, //1
	{0,0,1,0,0,1,0}, //2
	{0,0,0,0,1,1,0}, //3
 	{1,0,0,1,1,0,0}, //4
	{0,1,0,0,1,0,0}, //5
	{0,1,0,0,0,0,0}, //6
	{0,0,0,1,1,1,1}, //7
	{0,0,0,0,0,0,0}, //8
	{0,0,0,1,1,0,0}, //9	
	
};*/
void SevSeg_display_digit(volatile uint8_t *port,uint8_t dig);

void SevSeg_clear(volatile uint8_t *port);



#endif /* SEVSEG_H_ */