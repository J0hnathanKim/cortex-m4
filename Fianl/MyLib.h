/*
 * MyLib.h
 *
 *  Created on: 2015. 4. 6.
 *      Author: ESL-LKH
 */

#ifndef MYLIB_H_
#define MYLIB_H_
static unsigned char fnd_num[18] = {
//	0		1		2		3		4		5		6		7
	0x3f,	0x06,	0x5b,	0x4f,	0x66,	0x6d,	0x7d,	0x27,
//	8		9		a		b		c		d		e		f
	0x7f,	0x67,	0x5f,	0x7c,	0x39,	0x5e,	0x79,	0x71, 0x40,
//	off
	0x00
};

static unsigned char fnd_pos[6] = {
//	D1		D2		D3		D4		D5		D6
	0x01,	0x02,	0x04,	0x08,	0x10,	0x20
};

void WRITE_FND(int digit, int seg_out);
void WRITE_FND_DOT(int digit, int seg_out);
void FND_clear();
void FND_init();
void LED_init();
#define DLY_4 	16
#define DLY_8	8
#define DLY_16	4
#define DLY_32  2
#define N1 		61068
#define N2 		54421
#define N3 		48484
#define N4 		45844
#define N5 		40815
#define N6 		36363
#define N7 		(int)((36363+32388)/2)
#define N8 		30592


void BUZZER_init();
void TIMER_init();

void delay(int count);
void BUZZER_clear();
void Play(int freq);
void DelayForPlay(int DLY);
void WDTinitISR(void);
void PUSH_init();
void UART_init(float BRD, int BRDI, int BRDF);
char UART_getch(void);
char UART_getkey(void);
void UART_putch(uint8_t data);
void UART_putstr(char* pt);
void UART_printf(char *fmt, ...);
int Scale(int i);

#endif /* MYLIB_H_ */
