//처음 push switch 2번을 누르면 초기값 1 2 3 4 5 6 이 6개의 7segment에 표시된다.
//push switch 2번을 누를때마다 7segment 상에 있는 값이 좌측으로 쉬프트하게 된다.

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
	PUSH_init();
	DIP_init();
	FND_init();
	FND_clear();

	int nums[6] = {6,1,2,3,4,5};
    int i, j, tmp;
	while(1){
      if(start_flag == 1){
            for(i=0; i<6; i++){
                WRITE_FND(i+1, nums[i]);
            }
	    }
		push2_current = GPIO_READ(GPIO_PORTN, PIN3);

		if(push2_prev != 0 && push2_current == 0) {

			        tmp = nums[0];
			        for(i=0; i<5; i++){
			            nums[i] = nums[i+1];
			        }
			        nums[5] = tmp;
			        start_flag = 1;
		}

		push2_prev = push2_current;

		delay(10000);
	}
	return 0;
}



void delay(int count){
	while(count != 0){
		count--;
	}
}
