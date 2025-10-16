#include "cortex_m4.h"
#include "MyLib.h"


void delay(int count);

int main(void) {
	int push1_current, push4_current, push3_current;
	int push1_prev = 0, push4_prev = 0, push3_prev = 0;

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
			FND_clear();
		}


		if(push4_prev != 0 && push4_current == 0) {
			WRITE_FND(4,(0xF0 & dip_data) >> 4);
		}


		if(push3_prev != 0 && push3_current == 0) {
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
