#include "cortex_m4.h"
#include "MyLib.h"

#define FND_DIGITS 6

// UART_getkey()가 0을 리턴할 수 있으므로 입력이 들어올 때까지 대기하는 함수
static char wait_key(void) {
    char k;
    while ((k = UART_getkey()) == 0) { /* busy-wait */ }
    return k;
}

static int is_digit(char c) { return (c >= '0' && c <= '9'); }

int main(void) {
    uint32_t ui32SysClock;
    int BRD, BRDI, BRDF;

    // 120 MHz 시스템 클럭 설정
    ui32SysClock = SysCtlClockFreqSet(
        (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
        120000000
    );

    // UART0 초기화 (115200bps, 8-N-1)
    BRD  = ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)((BRD - BRDI) * 64 + 0.5);
    UART_init(BRD, BRDI, BRDF);

    // FND 초기화
    FND_init();
    FND_clear();

    UART_printf("\r\n=== UART FND d/n setter ===\r\n");
    UART_printf("Flow: d -> <pos 1~6> -> n -> <value 0~9>\r\n");
    UART_printf("Example: d2n7  (2번 자리에 7 표시)\r\n");

    while (1) {
        char c = UART_getkey();
        if (c == 0) continue;
        if (c != 'd' && c != 'D') continue;

        // d 입력 후 자리 입력 대기
        UART_printf("\r\nposition (1~6)? ");
        char dch = wait_key();
        UART_printf("%c", dch);

        if (!is_digit(dch)) { UART_printf("\r\nERR: not a digit."); continue; }
        int d = dch - '0';
        if (d < 1 || d > FND_DIGITS) { UART_printf("\r\nERR: position 1~6 only."); continue; }

        // n 키 입력 대기
        UART_printf("\r\npress 'n' then value (0~9)\r\n");
        char nch;
        do { nch = wait_key(); } while (nch != 'n' && nch != 'N');

        // 숫자 입력
        UART_printf("value (0~9)? ");
        char vch = wait_key();
        UART_printf("%c", vch);

        if (!is_digit(vch)) { UART_printf("\r\nERR: not a digit."); continue; }
        int v = vch - '0';
        if (v < 0 || v > 9) { UART_printf("\r\nERR: value 0~9 only."); continue; }

        // FND 표시
        WRITE_FND(d, v);
        UART_printf("\r\nOK: pos=%d, val=%d\r\n", d, v);
    }
}
