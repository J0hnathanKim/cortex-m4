/*
 * main.c
 * - 두 개의 DIP 스위치를 ON하면, 두 번호의 합(<=8)에 해당하는 한 개 LED만 ON
 *   (두 개가 아니거나 합이 9 이상이면 LED 모두 OFF)
 */

#include "cortex_m4.h"
#include "MyLib.h"

static void LED_clear(void);
static void delay(int count);

int main(void) {
    // 120 MHz 시스템 클럭 설정
    uint32_t ui32SysClock = SysCtlClockFreqSet(
        (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
        120000000
    );
    (void)ui32SysClock;

    unsigned char dip_data;

    LED_init();
    DIP_init();
    LED_clear();

    while (1) {
        // 8개 DIP를 8비트로 읽어오기 (비트0=1번 DIP, 비트7=8번 DIP)
        dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )   // PA3  -> bit0 (DIP1)
                 | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 )   // PA6  -> bit1 (DIP2)
                 | ( GPIO_READ(GPIO_PORTA, 0x80) >> 5 )   // PA7  -> bit2 (DIP3)
                 | ( GPIO_READ(GPIO_PORTB, 0x08)      )   // PB3  -> bit3 (DIP4)
                 | ( GPIO_READ(GPIO_PORTQ, 0x40) >> 2 )   // PQ6  -> bit4 (DIP5)
                 | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 )   // PQ5  -> bit5 (DIP6)
                 | ( GPIO_READ(GPIO_PORTQ, 0x10) << 2 )   // PQ4  -> bit6 (DIP7)
                 | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );  // PG6  -> bit7 (DIP8)

        // 1) 켜진 DIP의 개수(count)와 1-based 인덱스들의 합(sum) 계산
        int count = 0;
        int i;
        int sum = 0; // (DIP 번호의 합, 1~8)
        for (i = 0; i < 8; i++) {
            if (dip_data & (1u << i)) {
                count++;
                sum += (i + 1); // i는 0-based, DIP 번호는 1-based
            }
        }

        // 2) 정확히 두 개만 켜졌고, 합이 1~8 범위이면 그 번호 LED만 ON
        if (count == 2 && sum >= 1 && sum <= 8) {
            unsigned char led_mask = (1u << (sum - 1)); // 합에 해당하는 비트만 1
            // 하위 4비트 → PORTL, 상위 4비트 → PORTM
            GPIO_WRITE(GPIO_PORTL, 0x0F,  led_mask        & 0x0F);
            GPIO_WRITE(GPIO_PORTM, 0x0F, (led_mask >> 4)  & 0x0F);
        } else {
            // 조건 불충분(두 개 아님) 또는 합이 9 이상 → 모두 OFF
            GPIO_WRITE(GPIO_PORTL, 0x0F, 0x0);
            GPIO_WRITE(GPIO_PORTM, 0x0F, 0x0);
        }

        delay(2500000);
    }
}

static void LED_clear(void) {
    GPIO_WRITE(GPIO_PORTL, 0x0F, 0x0);
    GPIO_WRITE(GPIO_PORTM, 0x0F, 0x0);
    delay(2500000);
}

static void delay(int count) {
    while (count-- > 0) { /* busy-wait */ }
}

