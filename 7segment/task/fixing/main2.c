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

	// Store current display values for each digit (seg_out < 0 => blank/off)
	int fnd_values[4] = { -1, -1, -1, -1 }; // all OFF initially
    int i, d;
	while(1){
		 dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )
		                 | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 )
		                 | ( GPIO_READ(GPIO_PORTA, 0x80) >> 5 )
		                 | ( GPIO_READ(GPIO_PORTB, 0x08)      )
		                 | ( GPIO_READ(GPIO_PORTQ, 0x40) >> 2 )
		                 | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 )
		                 | ( GPIO_READ(GPIO_PORTQ, 0x10) << 2 )
		                 | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );

		push1_current = GPIO_READ(GPIO_PORTP, PIN1);
		push4_current = GPIO_READ(GPIO_PORTK, PIN7);
		push3_current = GPIO_READ(GPIO_PORTE, PIN5);

		if(push1_prev != 0 && push1_current == 0) {
			// When SW1 pressed, turn all digits off
			for(i=0;i<4;i++) fnd_values[i] = -1;
			FND_clear();
		}

		if(push4_prev != 0 && push4_current == 0) {
			// Save value for digit 4 (index 3)
			fnd_values[3] = (0xF0 & dip_data) >> 4;
		}

		if(push3_prev != 0 && push3_current == 0) {
			// Save value for digit 3 (index 2)
			fnd_values[2] = 0x0F & dip_data;
		}

		push1_prev = push1_current;
		push4_prev = push4_current;
		push3_prev = push3_current;

		// Multiplex: quickly refresh all digits
		for(d = 1; d <= 4; d++){
			WRITE_FND(d, fnd_values[d-1]); // seg_out < 0 => WRITE_FND will turn that digit off
		}
	}
	return 0;
}

void delay(int count){
	while(count != 0){
		count--;
	}
}
