//임베디드 시스템 교재 push switch 3번문제 푸는중,,,
//push_sw_2번 조건을 이리저리 바꿔보자,,,
#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);

int main(void) {
	int push1_current, push2_current, push3_current, push4_current;
	int push1_prev = 0, push2_prev = 0, push3_prev = 0, push4_prev = 0;
    int push1_flag = 0;
    int push2_flag = 0;
    int count = 0;
	uint32_t ui32SysClock;
	// Run from the PLL at 120 MHz.
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
											   SYSCTL_CFG_VCO_480), 120000000);
    DIP_init();
	PUSH_init();
	LED_init();
	int dip_data = 0;
	LED_clear();

	while(1){
		dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )   // PA3  -> bit0 (DIP1)
		                 | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 )   // PA6  -> bit1 (DIP2)
		                 | ( GPIO_READ(GPIO_PORTA, 0x80) >> 5 )   // PA7  -> bit2 (DIP3)
		                 | ( GPIO_READ(GPIO_PORTB, 0x08)      )   // PB3  -> bit3 (DIP4)
		                 | ( GPIO_READ(GPIO_PORTQ, 0x40) >> 2 )   // PQ6  -> bit4 (DIP5)
		                 | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 )   // PQ5  -> bit5 (DIP6)
		                 | ( GPIO_READ(GPIO_PORTQ, 0x10) << 2 )   // PQ4  -> bit6 (DIP7)
		                 | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );  // PG6  -> bit7 (DIP8)

		push1_current = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
		push2_current = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2
		push3_current = GPIO_READ(GPIO_PORTE, PIN5);  // PUSH_SW 3
		push4_current = GPIO_READ(GPIO_PORTK, PIN7); // PUSH_SW 4


		if(push1_prev != 0 && push1_current == 0) {
			push1_flag = 1;
			push2_flag = 0;
		}
		if(push1_flag == 1){
			GPIO_WRITE(GPIO_PORTL, 0xF, (dip_data & 0xF));
			GPIO_WRITE(GPIO_PORTM, 0xF, (dip_data >> 4) & 0xF);
			delay(1000000);
			GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);
			GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);
			delay(1000000);
		}


		if(push2_prev != 0 && push2_current == 0) {
			push1_flag = 0;
			push2_flag = 1;
		}

		if(push2_flag == 1){
			count += dip_data;
			GPIO_WRITE(GPIO_PORTL, 0xF, ( count & 0xF));
			GPIO_WRITE(GPIO_PORTM, 0xF, ( count >> 4) & 0xF);
			delay(1000000);
			if(count + dip_data > 255){
				count = 0;
			}

		}


		if(push3_prev != 0 && push3_current == 0) {
			push1_flag = 0;
			push2_flag = 0;
			GPIO_WRITE(GPIO_PORTL, 0xF, 0xF);
			GPIO_WRITE(GPIO_PORTM, 0xF, 0xF);
		}

		if(push4_prev != 0 && push4_current == 0) {
			push1_flag = 0;
			push2_flag = 0;
			GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);
			GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);
		}



		push1_prev = push1_current;
		push2_prev = push2_current;
		push3_prev = push3_current;
		push4_prev = push4_current;

		delay(10000);
	}
	return 0;
}

void LED_clear(){
	GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);
	GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);
	delay(2500000);
}

void delay(int count){
	while(count != 0){
		count--;
	}
}
