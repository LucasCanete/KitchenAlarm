
#include <SevenSegmentDisplay.h>


SevenSegmentDisplay::SevenSegmentDisplay(uint8_t a_pin, volatile uint8_t *a_dir,
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
                                        uint8_t ca4_pin, volatile uint8_t *ca4_dir):
                                        A_PIN_(a_pin), A_DIR_(a_dir),
                                        B_PIN_(b_pin), B_DIR_(b_dir),
                                        C_PIN_(c_pin), C_DIR_(c_dir),
                                        D_PIN_(d_pin), D_DIR_(d_dir),
                                        E_PIN_(e_pin), E_DIR_(e_dir),
                                        F_PIN_(f_pin), F_DIR_(f_dir),
                                        G_PIN_(g_pin), G_DIR_(g_dir),
                                        DPX_PIN_(dpx_pin), DPX_DIR_(dpx_dir),
                                        CA1_PIN_(ca1_pin), CA1_DIR_(ca1_dir),
                                        CA2_PIN_(ca2_pin), CA2_DIR_(ca2_dir),
                                        CA3_PIN_(ca3_pin), CA3_DIR_(ca3_dir),
                                        CA4_PIN_(ca4_pin), CA4_DIR_(ca4_dir)
                                        {


  A_PORT_ = (A_DIR_) + 1;
  B_PORT_ =  (B_DIR_) + 1;
  C_PORT_ =  (C_DIR_) + 1;
  D_PORT_ =  (D_DIR_) + 1;
  E_PORT_ =  (E_DIR_) + 1;
  F_PORT_ =  (F_DIR_) + 1;
  G_PORT_ =  (G_DIR_) + 1;
  DPX_PORT_ = (DPX_DIR_) + 1;
  CA1_PORT_ =  (CA1_DIR_) + 1;
  CA2_PORT_ =  (CA2_DIR_) + 1;
  CA3_PORT_ =  (CA3_DIR_) + 1;
  CA4_PORT_ =  (CA4_DIR_) + 1;

  //initialize array content

  ports[0] = A_PORT_;
  ports[1] = B_PORT_;
  ports[2] = C_PORT_;
  ports[3] = D_PORT_;
  ports[4] = E_PORT_;
  ports[5] = F_PORT_;
  ports[6] = G_PORT_;

  pins[0] = A_PIN_;
  pins[1] = B_PIN_;
  pins[2] = C_PIN_;
  pins[3] = D_PIN_;
  pins[4] = E_PIN_;
  pins[5] = F_PIN_;
  pins[6] = G_PIN_;


}


//All ports are outputs
void SevenSegmentDisplay::init(){
  *A_DIR_ |= (1 << A_PIN_);
  *B_DIR_ |= (1 << B_PIN_);
  *C_DIR_ |= (1 << C_PIN_);
  *D_DIR_ |= (1 << D_PIN_);
  *E_DIR_ |= (1 << E_PIN_);
  *F_DIR_ |= (1 << F_PIN_);
  *G_DIR_ |= (1 << G_PIN_);
  *DPX_DIR_ |= (1 << DPX_PIN_);
  *CA1_DIR_ |= (1 << CA1_PIN_);
  *CA2_DIR_ |= (1 << CA2_PIN_);
  *CA3_DIR_ |= (1 << CA3_PIN_);
  *CA4_DIR_ |= (1 << CA4_PIN_);
}





void SevenSegmentDisplay::first_digit(digit_state_t state){
  if(state == ON) (*CA1_PORT_) &= ~(1 << CA1_PIN_); //clear bit to turn digit on
  else if(state == OFF) (*CA1_PORT_) |= (1 << CA1_PIN_); //set bit to turn it off

  //rest of digits off
  (*CA2_PORT_) |= (1 << CA2_PIN_);
  (*CA3_PORT_) |= (1 << CA3_PIN_);
  (*CA4_PORT_) |= (1 << CA4_PIN_);


}

void SevenSegmentDisplay::second_digit(digit_state_t state){
  if(state == ON) (*CA2_PORT_) &= ~(1 << CA2_PIN_);
  else if (state == OFF) (*CA2_PORT_) |= (1 << CA2_PIN_);
  //rest of digits off
  (*CA1_PORT_) |= (1 << CA1_PIN_);
  (*CA3_PORT_) |= (1 << CA3_PIN_);
  (*CA4_PORT_) |= (1 << CA4_PIN_);
}

void SevenSegmentDisplay::third_digit(digit_state_t state){
  if(state == ON) (*CA3_PORT_) &= ~(1 << CA3_PIN_);
  else if (state == OFF) (*CA3_PORT_) |= (1 << CA3_PIN_);
  //rest of digits off
  (*CA1_PORT_) |= (1 << CA1_PIN_);
  (*CA2_PORT_) |= (1 << CA2_PIN_);
  (*CA4_PORT_) |= (1 << CA4_PIN_);

}

void SevenSegmentDisplay::fourth_digit(digit_state_t state){
  if(state == ON) (*CA4_PORT_) &= ~(1 << CA4_PIN_);
  else if (state == OFF) (*CA4_PORT_) |= (1 << CA4_PIN_);

  //rest of digits off
  (*CA1_PORT_) |= (1 << CA1_PIN_);
  (*CA2_PORT_) |= (1 << CA2_PIN_);
  (*CA3_PORT_) |= (1 << CA3_PIN_);

}

void SevenSegmentDisplay::all_digits_off(){
  (*CA1_PORT_) |= (1 << CA1_PIN_);
  (*CA2_PORT_) |= (1 << CA2_PIN_);
  (*CA3_PORT_) |= (1 << CA3_PIN_);
  (*CA4_PORT_) |= (1 << CA4_PIN_);

}

void SevenSegmentDisplay::decimal_point(digit_state_t state){

  if(state == ON) (*DPX_PORT_) |= (1 << DPX_PIN_); //set bit to turn it on

  else if(state == OFF) (*DPX_PORT_) &= ~(1 << DPX_PIN_); //clear bit to turn digit off
}


void SevenSegmentDisplay::show_value(uint8_t value){
  switch(value){
    case 0:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[0][i]==0) *ports[i] |= (1<<pins[i]);
  			else *ports[i] &= ~(1<<pins[i]);
  		}
		break;
		case 1:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[1][i]==0) *ports[i] |= (1<<pins[i]);
  			else *ports[i] &= ~(1<<pins[i]);
  		}
		break;
		case 2:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[2][i]==0) *ports[i] |= (1<< pins[i]);
  			else *ports[i] &= ~(1<<pins[i]);
  		}
		break;
		case 3:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[3][i]==0) *ports[i] |= (1 << pins[i]);
  			else *ports[i] &= ~(1<< pins[i]);
  		}
		break;
		case 4:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[4][i]==0) *ports[i] |= (1<< pins[i]);
  			else *ports[i] &= ~(1<< pins[i]);
  		}
		break;
		case 5:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[5][i]==0) *ports[i] |= (1<< pins[i]);
  			else *ports[i] &= ~(1<< pins[i]);
  		}
		break;
		case 6:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[6][i]==0) *ports[i] |= (1<< pins[i]);
  			else *ports[i] &= ~(1<< pins[i]);
  		}
		break;
		case 7:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[7][i]==0) *ports[i] |= (1<< pins[i]);
  			else *ports[i] &= ~(1<< pins[i]);
  		}
		break;
		case 8:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[8][i]==0) *ports[i] |= (1<<  pins[i]);
  			else *ports[i] &= ~(1<<  pins[i]);
  		}
		break;
		case 9:
  		for (uint8_t i = 0; i<7; i++){
  			if(segments[9][i]==0) *ports[i] |= (1<<  pins[i]);
  			else *ports[i] &= ~(1<<  pins[i]);
  		}
		break;
	}
}


void SevenSegmentDisplay::clear(){
    for (uint8_t i = 0; i<7; i++) *ports[i] &= ~(1<<  pins[i]);
}
