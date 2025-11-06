/*
 * UART_exam
 *
 */

#include "cortex_m4.h"
#include "myLib.h"

int main(void)
{
	uint8_t code;

	float BRD;
	int   BRDI;
	int   BRDF;

	uint32_t ui32SysClock;

	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
		                               SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
		                               SYSCTL_CFG_VCO_480), 120000000);

	//uart baud rate(p.1339)
	BRD = (float)ui32SysClock / (16 * 115200);
	BRDI = (int)BRD;
	BRDF = (int)(((BRD - (int)BRD)*64) + 0.5);

	UART_init(BRD, BRDI, BRDF);
	LED_init();
	int first = 0b1000;
	GPIO_WRITE(GPIO_PORTL,0xf,first);
	UART_printf("//**************************************//\n\r");
	UART_printf("//***********  Initialize  *************//\n\r");
	UART_printf("//***********      OK      *************//\n\r");
	UART_printf("//**************************************//\n\r");

	while(1) {
		code = UART_getkey();

		switch(code) {
			case 'a' :
				  first = (first >> 1);
				  if(first < 0b0001) first = 0b10000000;
				  GPIO_WRITE(GPIO_PORTL,0xf,(0xf&first));
				  GPIO_WRITE(GPIO_PORTM,0xf,0xf&(first>>4));
				  break;
			case 's' :
				  first = (first << 1);
				  if(first > 0b10000000) first = 0b00000001;
				  GPIO_WRITE(GPIO_PORTL,0xf,(0xf&first));
				  GPIO_WRITE(GPIO_PORTM,0xf,0xf&(first>>4));
				  break;
			default : break;
		}

	}

    return 0;
}




