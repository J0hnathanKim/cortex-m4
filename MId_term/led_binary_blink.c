//led 8개가 binary 값으로 업 카운팅 되면서 led가 점등된다.
//0부터 255까지 반복
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
        for (val = 1; val <= 255; val++) {
            // 하위 4비트 → Port L (PL0~3)
            GPIO_WRITE(GPIO_PORTL, 0x0F, val & 0x0F);

            // 상위 4비트 → Port M (PM0~3)
            GPIO_WRITE(GPIO_PORTM, 0x0F, (val >> 4) & 0x0F);

            // 잠깐 켜둠
            delay(2000000);
        }
    }
} // ← 이 부분이 빠졌음!

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