//LED 기본예제
//1~4번 led가 점등된 후 이 상태를 유지하며 5~8번 led가 점등된다. 이후에 1~4번 led가 꺼지고 5~8번 led가 꺼진다.
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

	while(1){
		GPIO_WRITE(GPIO_PORTL, 0xf, 0xf);
		delay(2500000);
		GPIO_WRITE(GPIO_PORTM, 0xf, 0xf);
		delay(2500000);
		GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
		delay(2500000);
		GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
		delay(2500000);
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

