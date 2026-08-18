#include<avr/io.h>
#include<avr/interrupt.h>
#include<avr/pgmspace.h>
#include<stdlib.h>
#include"lut.h"

uint32_t alpha_sort[3][90];
uint32_t alpha_est[3][90];
uint16_t alpha_cur[3][90];
int alpha_num = 7;
int alpha_num_cur = 7;
float ratio = 0.0;
int ampINT = 0;
float basfrq = 40.1;
int tcb_cnt = 0;
float PER;

float min(float a, float b){
    return (a < b) ? a : b;
}

void swap (uint32_t *x, uint32_t *y){
	uint32_t temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void shell_sort (void){
	int i, j, h, array_size;
	array_size = alpha_num * 12 + 6;
	for(h = 1; h <= array_size/9; h = 3*h + 1);
	for( ; h > 0; h /= 3){
		for (i = h; i < array_size; i++){
			j = i;
			while((j > h - 1) && (alpha_sort[0][j-h] > alpha_sort[0][j])) {
				swap(&alpha_sort[0][j-h], &alpha_sort[0][j]);
				swap(&alpha_sort[1][j-h], &alpha_sort[1][j]);
				swap(&alpha_sort[2][j-h], &alpha_sort[2][j]);
				j -= h;
			}
		}
	}
}

void makePER(const int alpha[][alpha_num], const int pole[], int amp){
    int array_cnt = 0;
	int array_cnt_sus = 0;
	PER = 2500000 / basfrq;
	while(array_cnt < alpha_num){
		alpha_sort[0][array_cnt_sus] = (uint32_t)((pgm_read_word(&alpha[amp][array_cnt])) * PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus] = 0;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((12000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((24000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 2;
		array_cnt++;
		array_cnt_sus++;
	}
	while(array_cnt > 0){
		array_cnt--;
		alpha_sort[0][array_cnt_sus] = (uint32_t)((18000 - pgm_read_word(&alpha[amp][array_cnt])) * PER / 36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus] = 0;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((30000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((42000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 2;
		array_cnt_sus++;
	}
	while(array_cnt < alpha_num){
		alpha_sort[0][array_cnt_sus] = (uint32_t)((18000 + pgm_read_word(&alpha[amp][array_cnt])) * PER / 36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus] = 0;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((30000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((42000 + pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 2;
		array_cnt++;
		array_cnt_sus++;
	}
	while(array_cnt > 0){
		array_cnt--;
		alpha_sort[0][array_cnt_sus] = (uint32_t)((36000 - pgm_read_word(&alpha[amp][array_cnt])) * PER / 36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus] = 0;
		alpha_sort[0][array_cnt_sus+alpha_num*4] = (uint32_t)((48000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*4] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*4] = 1;
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)((60000 - pgm_read_word(&alpha[amp][array_cnt]))*PER/36000) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+array_cnt_sus+1)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = 2;
		array_cnt_sus++;
	}
	for(int i=0; i<3; i++){
		alpha_sort[0][array_cnt_sus+alpha_num*8] = (uint32_t)(i * PER / 3) % (uint32_t)PER;
		alpha_sort[0][array_cnt_sus+alpha_num*8+3] = (uint32_t)((3 + i*2) * PER / 6) % (uint32_t)PER;
		alpha_sort[1][array_cnt_sus+alpha_num*8] = (pgm_read_byte(&pole[amp])+1)%2;
		alpha_sort[1][array_cnt_sus+alpha_num*8+3] = (pgm_read_byte(&pole[amp])+2)%2;
		alpha_sort[2][array_cnt_sus+alpha_num*8] = i;
		alpha_sort[2][array_cnt_sus+alpha_num*8+3] = i;
		array_cnt_sus++;
	}
	array_cnt = 0;
	shell_sort();
	for(int i = 0; i < alpha_num*12+6 - 1; i++){
		alpha_sort[0][i] = alpha_sort[0][i+1] - alpha_sort[0][i];
		if(alpha_sort[0][i] < 100){alpha_sort[0][i] = 100;}
	}
	alpha_sort[0][alpha_num*12+6 - 1] = (uint32_t)PER - alpha_sort[0][alpha_num*12+6 - 1];
	if(alpha_sort[0][alpha_num*12+6 - 1] < 100){alpha_sort[0][alpha_num*12+6 - 1] = 100;}
	for(int i = 0; i < alpha_num*12+6; i++){
		alpha_est[0][i] = alpha_sort[0][i];
		alpha_est[1][i] = alpha_sort[1][i];
		alpha_est[2][i] = alpha_sort[2][i];
	}
}

ISR(TCB0_INT_vect){
	TCB0.INTFLAGS = 0x1;
	if(tcb_cnt == 0){
		alpha_num_cur = alpha_num;
		for(int i = 0; i < alpha_num_cur*12+6; i++){
			alpha_cur[0][i] = (uint16_t)(alpha_est[0][i]);
			alpha_cur[1][i] = (uint16_t)(alpha_est[1][i]);
			alpha_cur[2][i] = (uint16_t)(alpha_est[2][i]);
		}
		PORTC.OUTSET = 0b00000010;
		PORTC.OUTCLR = 0b00000100;
	}
	if(alpha_cur[1][tcb_cnt] == 1){
		if(alpha_cur[2][tcb_cnt]==0){PORTC.OUTSET = 0b00000001;}
		else if(alpha_cur[2][tcb_cnt]==1){PORTC.OUTSET = 0b00000010;}
		else if(alpha_cur[2][tcb_cnt]==2){PORTC.OUTSET = 0b00000100;}
	}
	else if(alpha_cur[1][tcb_cnt] == 0){
		if(alpha_cur[2][tcb_cnt]==0){PORTC.OUTCLR = 0b00000001;}
		else if(alpha_cur[2][tcb_cnt]==1){PORTC.OUTCLR = 0b00000010;}
		else if(alpha_cur[2][tcb_cnt]==2){PORTC.OUTCLR = 0b00000100;}
	}
	TCB0.CCMP = alpha_cur[0][tcb_cnt]*4;
	tcb_cnt += 1;
	if(tcb_cnt > alpha_num_cur*12+6 - 1){tcb_cnt = 0;}
}
ISR(TCB1_INT_vect){
	//加速と変調率の計算
	TCB1.INTFLAGS = 0x1;
	if (~PORTD.IN & PIN4_bm){basfrq += 0.0004;}
	if (~PORTD.IN & PIN5_bm){basfrq -= 0.0004;}
	ratio = min(basfrq * 7.731 - 5.8903, 501);
}

int main(void){
    makePER(_7alpha, _7alpha_pole, ampINT);

	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0x0);

    TCB0.CCMP = 0x9C4;
	TCB0.CNT = 0x0;
	TCB0.CTRLB = 0b01000000;
	TCB0.EVCTRL = 0x0;
	TCB0.INTCTRL = 0x1;
	TCB0.CTRLA = 0x3;

	TCB1.CCMP = 0x9C4;
	TCB1.CNT = 0x0;
	TCB1.CTRLB = 0b01000000;
	TCB1.EVCTRL = 0x0;
	TCB1.INTCTRL = 0x1;
	TCB1.CTRLA = 0x1;

	PORTC.DIRSET = 0b00000111;
	PORTD.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTD.PIN5CTRL = PORT_PULLUPEN_bm;
    sei();
    while(1){
		if(basfrq >= 80){alpha_num = 0;}
		else if(basfrq >= 65){alpha_num = 2;}
		else if(basfrq >= 63){alpha_num = 3;}
		else if(basfrq >= 45){alpha_num = 4;}
		else if(basfrq >= 40){alpha_num = 5;}
    	else{}

		if(alpha_num == 7){ampINT = (uint16_t)ratio; makePER(_7alpha, _7alpha_pole, ampINT);}
		else if(alpha_num == 6){ampINT = (uint16_t)ratio; makePER(_6alpha, _6alpha_pole, ampINT);}
		else if(alpha_num == 5){ampINT = (uint16_t)ratio; makePER(_5alpha, _5alpha_pole, ampINT);}
		else if(alpha_num == 4){ampINT = (uint16_t)ratio; makePER(_4alpha, _4alpha_pole, ampINT);}
		else if(alpha_num == 3){ampINT = (uint16_t)ratio; makePER(_3alpha, _3alpha_pole, ampINT);}
		else if(alpha_num == 2){ampINT = (uint16_t)ratio; makePER(_2alpha, _2alpha_pole, ampINT);}
		else if(alpha_num == 1){ampINT = (uint16_t)ratio; makePER(_1alpha, _1alpha_pole, ampINT);}
		else if(alpha_num == 0){ampINT = (uint16_t)ratio; makePER(_1alpha, _1alpha_pole, 100);}
	}
}
