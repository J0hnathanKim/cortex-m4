// uart_buzzer_scale_main.c
#include "cortex_m4.h"
#include "MyLib.h"

int main(void) {
    uint32_t ui32SysClock;
    int BRD, BRDI, BRDF;

    // 120 MHz 시스템 클럭 설정
    ui32SysClock = SysCtlClockFreqSet(
        (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
        120000000
    );

    // UART0 초기화 (115200 8-N-1)
    BRD  = ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)((BRD - BRDI) * 64 + 0.5);
    UART_init(BRD, BRDI, BRDF);

    // 버저/타이머 준비 (MyLib 내부 구현 사용)
    TimerEn();

    UART_puts("\r\n=== UART Buzzer Scale ===\r\n");
    UART_puts("Press 1~7: Do Re Mi Fa Sol La Si\r\n");

    while (1) {
        char key = UART_getkey();
        if (!key) continue; // 논블로킹 폴링: 입력 없으면 루프 지속

        switch (key) {
            case '1':
                UART_puts("Do\r\n");
                Play(N1);               // '도'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            case '2':
                UART_puts("Re\r\n");
                Play(N2);               // '레'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            case '3':
                UART_puts("Mi\r\n");
                Play(N3);               // '미'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            case '4':
                UART_puts("Fa\r\n");
                Play(N4);               // '파'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            case '5':
                UART_puts("Sol\r\n");
                Play(N5);               // '솔'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            case '6':
                UART_puts("La\r\n");
                Play(N6);               // '라'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            case '7':
                UART_puts("Si\r\n");
                Play(N7);               // '시'
                DelayForPlay(DLY_4);
                BUZZEROFF();
                break;
            default:
                // 다른 키는 무시
                break;
        }
    }
    // return 0; // 도달하지 않음
}
