/*
 * MyLib.h
 *
 *  Created on: 2015. 4. 6.
 *      Author: ESL-LKH
 */

#ifndef MYLIB_H_
#define MYLIB_H_

/*  C1 : 도 , D1 : 레 , E1 : 미 ...
    DLY_4 : 4분음표 , DLY_8 : 8분음표 , DLY_16 : 16분음표     */
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

#endif /* MYLIB_H_ */
