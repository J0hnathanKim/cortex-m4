#include "cortex_m4.h"
#include "MyLib.h"

int main(void) {
    uint32_t ui32SysClock;
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                      SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                      SYSCTL_CFG_VCO_480), 120000000);

    FND_init();
    FND_clear();

    int nums[6] = {1,2,3,4,5,6};
    int i, j, tmp;

    while(1){
        // Multiplex 반복 횟수 더 줄임
        for(j=0; j<100; j++) {
            for(i=0; i<6; i++){
                WRITE_FND(i+1, nums[i]);
            }
        }

        // 좌측 시프트
        tmp = nums[0];
        for(i=0; i<5; i++){
            nums[i] = nums[i+1];
        }
        nums[5] = tmp;

        // 시프트 후 딜레이 더 줄임
        for(i=0; i<10; i++);
    }
    return 0;
}
