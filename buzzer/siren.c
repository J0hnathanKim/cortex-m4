#include "cortex_m4.h"
#include "MyLib.h"

void delay(int count);

int main(void) {
    int hz = 10000;   // 초기 주파수
    int i;
    uint32_t ui32SysClock;

    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_480), 120000000);
    BUZZER_init();

    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON

    while(1) {
        // 사이렌처럼 주파수가 30000에서 50000으로 증가 후 다시 30000으로 감소
        for(i = 0; i < 500; i++) {  // 반복 횟수 설정
            if(hz <= 10000 && hz >= 5000) {
                GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
                delay(hz);  // 주파수에 따라 딜레이 조정

                GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buzzer OFF
                delay(hz);  // 주파수에 따라 딜레이 조정

                hz += 50;  // 주파수 증가
                if (hz > 10000) hz = 10000;  // 최대 주파수 제한
            }
        }

        for(i = 0; i < 500; i++) {  // 반복 횟수 설정
            if(hz >= 5000 && hz <= 10000) {
                GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
                delay(hz);  // 주파수에 따라 딜레이 조정

                GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buzzer OFF
                delay(hz);  // 주파수에 따라 딜레이 조정

                hz -= 50;  // 주파수 감소
                if (hz < 5000) hz = 5000;  // 최소 주파수 제한
            }
        }
    }

    GPIO_WRITE(GPIO_PORTD, 0x10, ~0x10);  // buzzer OFF

    return 0;
}

void delay(int count) {
    while(count != 0) {
        count--;
    }
}
