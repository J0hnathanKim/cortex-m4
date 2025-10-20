/* 3번 push switch를 누르면 dip switch의 상위 4bit 값을 3번째 7segment에 16진수로 표시한다. 4번 push switch를 누르면 dip switch의 하위 4bit 값을 4번째 7segment에 16진수로 표시한다.
 1번 push switch를 누르면 덧셈연산 결과를 10진수로 7segment상에 표시한다.*/
#include "cortex_m4.h"
#include "MyLib.h"


void delay(int count);

int main(void) {
	int push1_current, push4_current, push3_current;
	int push1_prev = 0, push4_prev = 0, push3_prev = 0;
	int push3_flag = 0;
	int push4_flag = 0;
	int push1_flag = 0;
	int sum = 0;
	int i, j;

	uint32_t ui32SysClock;
	// Run from the PLL at 120 MHz.
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
											   SYSCTL_CFG_VCO_480), 120000000);

	PUSH_init();
	DIP_init();
	FND_init();
	FND_clear();
    int dip_data = 0;

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
		push4_current = GPIO_READ(GPIO_PORTK, PIN7);  // PUSH_SW 4
		push3_current = GPIO_READ(GPIO_PORTE, PIN5);  // PUSH_SW 3


		if(push1_prev != 0 && push1_current == 0) {
			push3_flag = 0;
			push4_flag = 0;
			FND_clear();
			sum = (0x0F & dip_data) + ((0xF0 & dip_data) >> 4);
			i = sum / 10;
			j = sum % 10;
			push1_flag = 1;
		}
		if(push1_flag == 1){
			WRITE_FND(5,i);
			WRITE_FND(6,j);
		}


		if(push4_prev != 0 && push4_current == 0) {
			push1_flag = 0;
			push4_flag = 1;
		}
		if(push4_flag == 1){
			WRITE_FND(4,(0xF0 & dip_data) >> 4);
		}


		if(push3_prev != 0 && push3_current == 0) {
			push1_flag = 0;
			push3_flag = 1;
		}
		if(push3_flag == 1){
			WRITE_FND(3, 0x0F & dip_data);
		}


		push1_prev = push1_current;
		push4_prev = push4_current;
		push3_prev = push3_current;

		delay(10000);
	}
	return 0;
}



void delay(int count){
	while(count != 0){
		count--;
	}
}
