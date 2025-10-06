#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);

int main(void) {
    int count = 0;
    uint32_t ui32SysClock;
    int push_data;

    // Run from the PLL at 120 MHz.
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                   SYSCTL_CFG_VCO_480), 120000000);

    PUSH_init();
    LED_init();

    LED_clear();

    while(1) {
        // 각 스위치의 active low 특성에 맞게 상태를 개별적으로 추출
        push_data = 0;
        if (!(GPIO_READ(GPIO_PORTP, 0x02))) push_data |= 0x01; // 1번 스위치(카운트업)
        if (!(GPIO_READ(GPIO_PORTN, 0x08))) push_data |= 0x02; // 2번 스위치(카운트다운)
        if (!(GPIO_READ(GPIO_PORTE, 0x20))) push_data |= 0x04; // 3번 스위치(LED OFF)
        if (!(GPIO_READ(GPIO_PORTK, 0x80))) push_data |= 0x08; // 4번 스위치(LED ON)
        /*
        push_data =
         (!GPIO_READ(GPIO_PORTP, 0x02) << 0) |
         (!GPIO_READ(GPIO_PORTN, 0x08) << 1) |
         (!GPIO_READ(GPIO_PORTE, 0x20) << 2) |
         (!GPIO_READ(GPIO_PORTK, 0x80) << 3);
            */

        // 1번 스위치: 카운트업 (1~15)
        if(push_data & 0x01) {
            for (count = 1; count <= 15; count++) {
                GPIO_WRITE(GPIO_PORTL, 0x0F, count & 0x0F);
                delay(2000000);
            }
            // 스위치 릴리즈 대기(반복 방지) -> 버튼을 누르고 있을때 알아서 카운트업 또는 다운 되는 동작을 원한다면 필요없음.
            //while (!(GPIO_READ(GPIO_PORTP, 0x02)));
        }

        // 2번 스위치: 카운트다운 (15~1)
        if(push_data & 0x02) {
            for (count = 15; count >= 1; count--) {
                GPIO_WRITE(GPIO_PORTL, 0x0F, count & 0x0F);
                delay(2000000);
            }
            //while (!(GPIO_READ(GPIO_PORTN, 0x08)));
        }

        // 3번 스위치: LED 전체 OFF
        if(push_data & 0x04) {
            GPIO_WRITE(GPIO_PORTL, 0x0F, 0x00);
            //while (!(GPIO_READ(GPIO_PORTE, 0x20)));
        }

        // 4번 스위치: LED 전체 ON
        if(push_data & 0x08) {
            GPIO_WRITE(GPIO_PORTL, 0x0F, 0x0F);
            //while (!(GPIO_READ(GPIO_PORTK, 0x80)));
        }
    }
    return 0;
}

void LED_clear() {
    GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
    GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
    delay(2500000);
}

void delay(int count) {
    while(count != 0) {
        count--;
    }
}
