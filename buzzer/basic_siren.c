#include "cortex_m4.h"
#include "MyLib.h"

void delay(int count);

int main(void){
    int i;

    uint32_t ui32SysClock;

    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN   |
                                       SYSCTL_USE_PLL    |
                                       SYSCTL_CFG_VCO_480),
                                       120000000);

    BUZZER_init();

    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON (PD4 = 1)

    // 톤 파라미터 (필요하면 숫자만 바꿔서 미세 조정)
    const int HI_HALF_DELAY = 22000;  // "삐" (짧을수록 높은 음)
    const int LO_HALF_DELAY = 44000;  // "뽀" (길수록 낮은 음)
    const int HI_CYCLES     = 420;    // "삐" 유지 길이(토글 횟수)
    const int LO_CYCLES     = 420;    // "뽀" 유지 길이(토글 횟수)
    const int GAP_DELAY     = 120000; // 톤 사이 살짝 쉬는 시간

    while(1){
        // ===== 높은 톤: 삐 =====
        for(i = 0; i < HI_CYCLES; i++){
            GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);   // PB2 = 1
            delay(HI_HALF_DELAY);
            GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // PB2 = 0 (마스크에 의해 해당 비트만 0)
            delay(HI_HALF_DELAY);
        }
        delay(GAP_DELAY);

        // ===== 낮은 톤: 뽀 =====
        for(i = 0; i < LO_CYCLES; i++){
            GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);   // PB2 = 1
            delay(LO_HALF_DELAY);
            GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // PB2 = 0
            delay(LO_HALF_DELAY);
        }
        delay(GAP_DELAY);
    }

    GPIO_WRITE(GPIO_PORTD, 0x10, ~0x10);  // buzzer OFF (여긴 도달하지 않음)
    return 0;
}

void delay(int count){
    while(count != 0){
        count--;
    }
}
