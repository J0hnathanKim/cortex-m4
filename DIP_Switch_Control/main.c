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
	 DIP_init();
	 LED_clear();
	 while(1){

	        // Read Dip Sw
	        /*
	            //PA3
	            //■■□□
	            //3 2 1 0
	            //8 4 2 1

	        dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )
	                  | ( GPIO_READ(GPIO_PORTA, 0x20) >> 5 )
	                  | ( GPIO_READ(GPIO_PORTA, 0x40) >> 4 )
	                  | ( GPIO_READ(GPIO_PORTB, 0x08) << 1 ) 
	                  | ( GPIO_READ(GPIO_PORTQ, ????) >> ? )
	                  | ( GPIO_READ(GPIO_PORTQ, ????) >> ? )
	                  | ( GPIO_READ(GPIO_PORTQ, ????) >> ? )
	                  | ( GPIO_READ(GPIO_PORTG, ????) >> ? );
	        */

	        // LED
	        GPIO_WRITE(GPIO_PORTL, 0xF,  (dip_data & 0xF));
	        GPIO_WRITE(GPIO_PORTM, 0xF, ((dip_data >> 4) & 0xF));
	        delay(2500000);
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



