#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H
#include<util/delay.h>
/*
 4 digits Seven Segment display
 July 2025
 Lucas Canete
*/

typedef enum {
  ON,
  OFF
} digit_state_t;

#include <stdint.h>

class SevenSegmentDisplay{

    public:
      SevenSegmentDisplay(uint8_t a_pin, volatile uint8_t *a_dir,
                          uint8_t b_pin, volatile uint8_t *b_dir,
                          uint8_t c_pin, volatile uint8_t *c_dir,
                          uint8_t d_pin, volatile uint8_t *d_dir,
                          uint8_t e_pin, volatile uint8_t *e_dir,
                          uint8_t f_pin, volatile uint8_t *f_dir,
                          uint8_t g_pin, volatile uint8_t *g_dir,
                          uint8_t dpx_pin, volatile uint8_t *dpx_dir,
                          uint8_t ca1_pin, volatile uint8_t *ca1_dir,
                          uint8_t ca2_pin, volatile uint8_t *ca2_dir,
                          uint8_t ca3_pin, volatile uint8_t *ca3_dir,
                          uint8_t ca4_pin, volatile uint8_t *ca4_dir);

      void init();

      void first_digit(digit_state_t state);

      void second_digit(digit_state_t state);

      void third_digit(digit_state_t state);

      void fourth_digit(digit_state_t state);

      void all_digits_off();

      void decimal_point(digit_state_t state);

      void show_value(uint8_t value);

      void clear();

    private:


        uint8_t segments[10][7] = {
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

        };

        uint8_t A_PIN_;
        volatile uint8_t *A_DIR_ ;
        volatile uint8_t *A_PORT_ ;

        uint8_t B_PIN_;
        volatile uint8_t *B_DIR_ ;
        volatile uint8_t *B_PORT_ ;

        uint8_t C_PIN_;
        volatile uint8_t *C_DIR_;
        volatile uint8_t *C_PORT_ ;

        uint8_t D_PIN_;
        volatile uint8_t *D_DIR_;
        volatile uint8_t *D_PORT_ ;

        uint8_t E_PIN_;
        volatile uint8_t *E_DIR_;
        volatile uint8_t *E_PORT_ ;

        uint8_t F_PIN_;
        volatile uint8_t *F_DIR_;
        volatile uint8_t *F_PORT_ ;

        uint8_t G_PIN_;
        volatile uint8_t *G_DIR_ ;
        volatile uint8_t *G_PORT_ ;

        uint8_t DPX_PIN_;
        volatile uint8_t *DPX_DIR_;
        volatile uint8_t *DPX_PORT_ ;

        uint8_t CA1_PIN_;
        volatile uint8_t *CA1_DIR_ ;
        volatile uint8_t *CA1_PORT_ ;

        uint8_t CA2_PIN_;
        volatile uint8_t *CA2_DIR_ ;
        volatile uint8_t *CA2_PORT_ ;

        uint8_t CA3_PIN_;
        volatile uint8_t *CA3_DIR_;
        volatile uint8_t *CA3_PORT_ ;

        uint8_t CA4_PIN_;
        volatile uint8_t *CA4_DIR_;
        volatile uint8_t *CA4_PORT_ ;

        volatile uint8_t *ports[7];

        uint8_t pins[7];


};


#endif
