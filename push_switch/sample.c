
#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);

int main(void) {
	int push_data;
	char count = 0;
	int flag = 0;

	uint32_t ui32SysClock;
	// Run from the PLL at 120 MHz.
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
											   SYSCTL_CFG_VCO_480), 120000000);

	PUSH_init();
	LED_init();

	LED_clear();

	while(1){
		push_data = GPIO_READ(GPIO_PORTP,0x02) >> 1;
		/*
		if()
			LED count decrease
		if()
			LED off
		if()
		 	LED on
		*/
		delay(900000);
	}
	return 0;
}

void LED_clear(){
	GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
	GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
	delay(2500000);
}

void delay(int count){
	while(count != 0){
		count--;
	}
}
