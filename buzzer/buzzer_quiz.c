#include "cortex_m4.h"
#include "MyLib.h"


void BUZZERON();
void BUZZEROFF();
void PLAY(int freq) ;
void TimerEn() ;
int push1_current, push2_current;
int push1_prev = 0;
int push2_prev = 0;
int play_music[64] = { 20, 20, 20, 20, 20, 60, 20, 40, 40, 40, 20,

	20, 60, 20, 80, 20, 40, 40, 40, 20, 20, 40,

	40, 20, 20, 40, 20, 20, 20, 20, 40, 40,

	20, 20, 20, 20, 40, 20, 40, 40, 40, 20,

	20, 40, 20, 20, 80, 20, 40, 40, 20, 20,

	40, 40, 20, 20, 40, 20, 20, 20, 20, 20,

	120, 80 };

	int music = 0;

	int buzzer[8] = {N1, N2, N3, N4, N5, N6, N7, N8};

int main(void){

	int buzzer_cnt = 0;
	int buzzer_count = 0;

	uint32_t ui32SysClock;


	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
									   SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
									   SYSCTL_CFG_VCO_480), 120000000);
	PUSH_init();
	TIMER_init();
	BUZZER_init();
	WDTinitISR();
	LED_init();

	GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON
	GPTMCTL = GPTMCTL | 0x41;   		 // timer enable

	BUZZER_clear();


	int i = -1;
	while(1)
	{
	    push1_current = GPIO_READ(GPIO_PORTP, PIN1);
	    push2_current = GPIO_READ(GPIO_PORTN, PIN3);  // 일단 주석


	    if(push1_prev != 0 && push1_current == 0) {
	    	i += 1;
	    	GPIO_WRITE(GPIO_PORTL, 0xf, i);
	    	GPIO_WRITE(GPIO_PORTM, 0xf, (i>>4));
	        Play(buzzer[i%8]);
	        DelayForPlay(DLY_8);
	        BUZZER_clear();
	    }

	    if(push2_prev != 0 && push2_current == 0){
	    	i -= 1;
	    	if(i < 0) i += 8;
	        GPIO_WRITE(GPIO_PORTL, 0xf, i);
	       	GPIO_WRITE(GPIO_PORTM, 0xf, (i>>4));
	    	Play(buzzer[i%8]);
	    	DelayForPlay(DLY_8);
	        BUZZER_clear();
	    }

	    push1_prev = push1_current;
	    push2_prev = push2_current;
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




