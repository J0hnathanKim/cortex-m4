// uart_led_shift.c
#include "cortex_m4.h"
#include "MyLib.h"

static void show_leds(uint8_t v) {
    // 하위 4비트 -> Port L (LED1~4), 상위 4비트 -> Port M (LED5~8)
    GPIO_WRITE(GPIO_PORTL, 0x0F, (v & 0x0F));
    GPIO_WRITE(GPIO_PORTM, 0x0F, ((v >> 4) & 0x0F));
}

int main(void) {
    uint32_t ui32SysClock;
    int BRD, BRDI, BRDF;

    // 120 MHz 시스템 클럭
    ui32SysClock = SysCtlClockFreqSet(
        (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
        120000000
    );

    // UART0 115200 8-N-1
    BRD  = ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)((BRD - BRDI) * 64 + 0.5);
    UART_init(BRD, BRDI, BRDF);

    // LED 포트 초기화
    LED_init();

    // 초기 상태: LED4만 ON (비트기준: LED1=bit0 … LED8=bit7)
    // LED4 => bit3
    uint8_t led = (1U << 3);
    show_leds(led);

    UART_puts("\r\n=== UART LED Shift ===\r\n");
    UART_puts("a: shift left (circular), s: shift right (circular)\r\n");

    while (1) {
        char key = UART_getkey();
        if (!key) continue;   // 입력 없으면 계속 폴링

        if (key == 'a' || key == 'A') {
            // 왼쪽 순환 쉬프트 (8비트)
            led = (uint8_t)(((led << 1) | (led >> 7)) & 0xFF);
            show_leds(led);
        } else if (key == 's' || key == 'S') {
            // 오른쪽 순환 쉬프트 (8비트)
            led = (uint8_t)(((led >> 1) | (led << 7)) & 0xFF);
            show_leds(led);
        }
        // 다른 키는 무시
    }
}
