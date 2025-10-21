#include "cortex_m4.h"
#include "MyLib.h"


void BUZZERON();
void BUZZEROFF();
void PLAY(int freq) ;
void TimerEn() ;
int play_music[64] = { 20, 20, 20, 20, 20, 60, 20, 40, 40, 40, 20,

	20, 60, 20, 80, 20, 40, 40, 40, 20, 20, 40,

	40, 20, 20, 40, 20, 20, 20, 20, 40, 40,

	20, 20, 20, 20, 40, 20, 40, 40, 40, 20,

	20, 40, 20, 20, 80, 20, 40, 40, 20, 20,

	40, 40, 20, 20, 40, 20, 20, 20, 20, 20,

	120, 80 };

	int music = 0;

	int buzzer[63] = { N4, N5, N6, N4, N8, N6, N5, N8, N5, N4,

	N2, N6, N4, N3, 0, N3, N2, N3, N4, N5, N1,

	N4, N5, N6, N7, N7, N6, N5, N4, N5, 0,

	N4, N5, N6, N4, N8, N6, N5, N8, N5, N4,

	N2, N2, N3, N4, N1, 0, N1, N2, N3, N4,

	N5, N1, N4, N5, N6, N7, N7, N6, N5, N4,

	N4, 0 };

int main(void){

	int buzzer_cnt = 0;
	int buzzer_count = 0;

	uint32_t ui32SysClock;


	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
									   SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
									   SYSCTL_CFG_VCO_480), 120000000); //프리스케일링
	TIMER_init();
	BUZZER_init();
	WDTinitISR();

	GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON
	GPTMCTL = GPTMCTL | 0x41;   		 // timer enable

	BUZZER_clear();

/*
	Play(C1);
	DelayForPlay(DLY_8);

	Play(D1);
	DelayForPlay(DLY_8);

	Play(E1);
	DelayForPlay(DLY_8);

	Play(F1);
	DelayForPlay(DLY_8);

	Play(G1);
	DelayForPlay(DLY_8);

	Play(A1);
	DelayForPlay(DLY_8);

	Play(B1);
	DelayForPlay(DLY_8);

	Play(C2);
	DelayForPlay(DLY_8);
	*/
	int i, cnt, flag;
	while(1)
		{
			for(i = 0; i < cnt / 100; i++)
			{
				Play(10000+cnt);
				DelayForPlay(1);

				Play(10000+cnt);
				DelayForPlay(1);
			}

			if(flag) cnt += 50; else cnt -= 50;
			if(cnt >= 5000) flag = 0;   // Decrease cnt
			if(cnt <= 100) flag=1;   	// Increase cnt
		}
    return 0;
}
void TimerEn() {
// buzzer ON
	GPTMCTL = GPTMCTL | 0x41;   		 // timer enable

	BUZZER_clear();
}
void BUZZERON() {
	GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);
}

void BUZZEROFF() {
	GPIO_WRITE(GPIO_PORTD, 0x10, ~0x10);  // buzzer OFF
	WDT1LOAD = 0xFFFFFFFF;
}

void PLAY(int freq) {
	GPTMTAILR = buzzer[freq];
	GPTMTMATCHR = buzzer[freq] / 2;
}




