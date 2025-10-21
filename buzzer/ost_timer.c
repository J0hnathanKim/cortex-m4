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


	Play(N1);
	DelayForPlay(DLY_8);

	Play(N2);
	DelayForPlay(DLY_8);

	Play(N3);
	DelayForPlay(DLY_8);

	Play(N4);
	DelayForPlay(DLY_8);

	Play(N5);
	DelayForPlay(DLY_8);

	Play(N6);
	DelayForPlay(DLY_8);

	Play(N7);
	DelayForPlay(DLY_8);

	Play(N8);
	DelayForPlay(DLY_8);

	int i;
	while(1)
		{
		    for(i = 0; i < 63; i++){
		    	Play(buzzer[i]);
		    	DelayForPlay(DLY_8);
		    }

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




