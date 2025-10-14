#include "cortex_m4.h"
#include "MyLib.h"

int main(void) {

	uint32_t ui32SysClock;

	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
										SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
										SYSCTL_CFG_VCO_480), 120000000);

	FND_init();
	FND_clear();

	while(1){
		WRITE_FND(1,2);          // FND1: 2
		WRITE_FND_DOT(2,1);      // FND2: 1 + 점
		WRITE_FND(3,8);          // FND3: 8
		WRITE_FND_DOT(4,5);      // FND4: 5 + 점
		WRITE_FND(5,1);          // FND5: 1
		WRITE_FND(6,2);          // FND6: 2
	}

	return 0;
}
