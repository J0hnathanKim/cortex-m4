/*
 * main.c
*/

#include "cortex_m4.h"
#include "MyLib.h"


int main(void) {

	uint32_t ui32SysClock;

	// Run from the PLL at 120 MHz.
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
										SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
										SYSCTL_CFG_VCO_480), 120000000);

	FND_init();
	FND_clear();

	while(1){
		WRITE_FND(1,1);
		WRITE_FND_DOT(2,2);
		WRITE_FND(3,3);
		WRITE_FND_DOT(4,4);
		WRITE_FND(5,5);
		WRITE_FND_DOT(6,6);
	}

	return 0;
}




