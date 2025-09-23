#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int);

int main(void){

	uint32_t g_ui32SysClock;

	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
									   	 SYSCTL_OSC_MAIN   |
									     SYSCTL_USE_PLL    |
									     SYSCTL_CFG_VCO_480),
									     120000000);

	LED_init();
	LED_clear();

int val;   
while(1){
    // up count from 1 to 15
    for (val = 1; val <= 15; val++) {
        GPIO_WRITE(GPIO_PORTL, 0x0F, val & 0x0F);
        delay(2000000);
    }

    // down count from 14 to 1
    for (val = 14; val >= 1; val--) {
        GPIO_WRITE(GPIO_PORTL, 0x0F, val & 0x0F);
        delay(2000000);
    }
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
