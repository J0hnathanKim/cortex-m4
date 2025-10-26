#include "cortex_m4.h"
#include "MyLib.h"


void BUZZERON();
void BUZZEROFF();
void PLAY(int freq) ;
void TimerEn() ;
int push1_current, push2_current, push3_current;
int push1_prev = 0;
int push2_prev = 0;
int push3_prev = 0;
int play_music[64] = { 20, 20, 20, 20, 20, 60, 20, 40, 40, 40, 20,

	20, 60, 20, 80, 20, 40, 40, 40, 20, 20, 40,

	40, 20, 20, 40, 20, 20, 20, 20, 40, 40,

	20, 20, 20, 20, 40, 20, 40, 40, 40, 20,

	20, 40, 20, 20, 80, 20, 40, 40, 20, 20,

	40, 40, 20, 20, 40, 20, 20, 20, 20, 20,

	120, 80 };

	int music = 0;

	int buzzer[8] = {N1, N2, N3, N4, N5, N6, N7, N8};

	int melody[63] =  { N4, N5, N6, N4, N8, N6, N5, N8, N5, N4,

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
									   SYSCTL_CFG_VCO_480), 120000000);
	PUSH_init();
	TIMER_init();
	BUZZER_init();
	WDTinitISR();
	LED_init();

	GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON
	GPTMCTL = GPTMCTL | 0x41;   		 // timer enable

	BUZZER_clear();

    int j = 0;
	int i = -1;
	while(1)
	{
	    push1_current = GPIO_READ(GPIO_PORTP, PIN1);
	    push2_current = GPIO_READ(GPIO_PORTN, PIN3);
	    push3_current = GPIO_READ(GPIO_PORTE, PIN5);


	    if(push1_prev != 0 && push1_current == 0) {
	    	i += 1;
	    	i %= 8;
	    	GPIO_WRITE(GPIO_PORTL, 0xf, i);
	    	GPIO_WRITE(GPIO_PORTM, 0xf, (i>>4));
	        Play(buzzer[i]);
	        DelayForPlay(DLY_8);
	        BUZZER_clear();
	    }

	    if(push2_prev != 0 && push2_current == 0){
	    	i -= 1;
	    	if(i < 0) i += 8;
	    	i %= 8;
	        GPIO_WRITE(GPIO_PORTL, 0xf, i);
	       	GPIO_WRITE(GPIO_PORTM, 0xf, (i>>4));
	    	Play(buzzer[i]);
	    	DelayForPlay(DLY_8);
	        BUZZER_clear();
	    }

	    if(push3_prev != 0 && push3_current == 0){
	    	for(j = 0; j < 63; j++){
	    			    	Play(melody[j]);
	    			    	DelayForPlay(DLY_8);
	    			    }
	    }

	    push1_prev = push1_current;
	    push2_prev = push2_current;
	    push3_prev = push3_current;
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




