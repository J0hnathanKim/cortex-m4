/*DIP_SW 7번을 ON하면 FND 5번, 6번을 이용하여  십진수 2자리 
up_counting을 한다. 십진수 99에 도달하면 다시 0이 된다. 이 상태에서 DIP_SW 7
번을 off하고 DIP_SW 8번을 ON하면  down_counting한다. */

#include "cortex_m4.h"
#include "MyLib.h"

void delay(int count);


int main(void) {
	int push2_current;
	int push2_prev = 0;


	uint32_t ui32SysClock;
	// Run from the PLL at 120 MHz.
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
											   SYSCTL_CFG_VCO_480), 120000000);
    int start_flag = 0;
	DIP_init();
	FND_init();
	FND_clear();
    int dip_data = 0;

    int i = 0;
    int j = 0;
    int k;
	while(1){
		 dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )
				                 | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 )
				                 | ( GPIO_READ(GPIO_PORTA, 0x80) >> 5 )
				                 | ( GPIO_READ(GPIO_PORTB, 0x08)      )
				                 | ( GPIO_READ(GPIO_PORTQ, 0x40) >> 2 )
				                 | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 )
				                 | ( GPIO_READ(GPIO_PORTQ, 0x10) << 2 )
				                 | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );

       if(start_flag == 1){
    	   for(k = 0; k < 500; k++){
                WRITE_FND(5, i);
                WRITE_FND(6, j);
    	   }

	    }


		if(dip_data == 0x40) {

			if(10*i + j >= 99){
				i = 0;
				j = 0;
			}
			else if(j >= 9){
				j = 0;
				i += 1;
			}
			else j += 1;
			start_flag = 1;

		}
		else if(dip_data == 0x80){
             if(i*10 + j <= 0){
            	 i = 9;
            	 j = 9;
             }
             else if(j <= 0) {
                        	 j = 9;
                        	 i -= 1;
             }
             else j -= 1;
             start_flag = 1;

		}

		delay(1000000);

	}
	return 0;
}

void delay(int count){
	while(count != 0){
		count--;
	}
}




