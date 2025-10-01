/*
 * main.c
 */

#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);

int main(void) {

	uint32_t ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
												SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
												SYSCTL_CFG_VCO_480), 120000000);

	unsigned char dip_data;

	LED_init();
	DIP_init();
	LED_clear();

	while(1){
		// Read Dip Data
		dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )    
		           | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 )   
		           | ( GPIO_READ(GPIO_PORTA, 0x80) >> 5 )   
		           | ( GPIO_READ(GPIO_PORTB, 0x08))   
		           | ( GPIO_READ(GPIO_PORTQ, 0x40) >> 2 )   
		           | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 )   
		           | ( GPIO_READ(GPIO_PORTQ, 0x10) << 2 )   
		           | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );

		//if(dip_data & 0x01) cnt += 1;
		//if(dip_data & 0x80) cnt -= 1;
		// LED
		//GPIO_WRITE(GPIO_PORTL, 0xf, (cnt&0xf));
		//GPIO_WRITE(GPIO_PORTM, 0xf, (cnt>>4) & 0xf);
		//delay(2500000);
	}

}

void LED_clear(){
	GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
	GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
	delay(2500000);
}

void delay(int count){
	while (count != 0) {
		count--;
	}
}
