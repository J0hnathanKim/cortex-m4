#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);

int main(void) {
    uint32_t push_data;
    int en1 = 0, en2 = 0, en3 = 0; // 각 스위치의 flag
    uint8_t led_count = 0;  // 8비트 카운터

    uint32_t ui32SysClock;
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_480), 120000000);

    PUSH_init();
    LED_init();

    LED_clear();

    while(1) {
        // 4개의 포트에 스위치가 연결됐다고 가정 (각 포트에 1개씩만 사용)
        // push_data의 각 비트: 0이면 off, 1이면 on (active low이므로 반전)
        push_data = ((~GPIO_READ(GPIO_PORTP, 0x02) >> 1) & 0x01) |    // PUSH_SW 1 (bit0)
                    ((~GPIO_READ(GPIO_PORTN, 0x08) >> 2) & 0x02) |    // PUSH_SW 2 (bit1)
                    ((~GPIO_READ(GPIO_PORTE, 0x20) >> 3) & 0x04) |    // PUSH_SW 3 (bit2)
                    ((~GPIO_READ(GPIO_PORTK, 0x80) >> 4) & 0x08);     // (미사용, bit3)

        // PUSH_SW 1: Turn ON LED 1-4 (PORT L)
        if (!en1) { // 대기상태
            if (push_data & 0x01) { // push1 눌림
                GPIO_WRITE(GPIO_PORTL, 0xF, 0xF);
                en1 = 1;
            }
        } else if (en1 && !(push_data & 0x01)) { // push1 떨어짐
            en1 = 0;
        }

        // PUSH_SW 2: Binary up counting on LED 1-8
        if (!en2) {
            if (push_data & 0x02) { // push2 눌림
                led_count++;
                GPIO_WRITE(GPIO_PORTL, 0xF, led_count & 0xF);
                GPIO_WRITE(GPIO_PORTM, 0xF, (led_count >> 4) & 0xF);
                en2 = 1;
            }
        } else if (en2 && !(push_data & 0x02)) { // push2 떨어짐
            en2 = 0;
        }

        // PUSH_SW 3: Turn OFF all LEDs
        if (!en3) {
            if (push_data & 0x04) { // push3 눌림
                GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);
                GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);
                led_count = 0;
                en3 = 1;
            }
        } else if (en3 && !(push_data & 0x04)) { // push3 떨어짐
            en3 = 0;
        }

        delay(50000); // debounce
    }
    return 0;
}

void LED_clear(){
    GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);
    GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);
    delay(2500000);
}

void delay(int count){
    while(count--);
}
