#include "cortex_m4.h"
#include "MyLib.h"

// 각 음계의 주기 값 (Timer Load 값)
#define N1 3822   // 도
#define N2 3405   // 레
#define N3 3034   // 미
#define N4 2863   // 파
#define N5 2551   // 솔
#define N6 2272   // 라
#define N7 2024   // 시

void Play(int freq);
void DelayForPlay(unsigned int dly);

int main(void) {
    uint32_t ui32SysClock;
    int BRD, BRDI, BRDF;
    char key;

    // 시스템 클럭 120 MHz 설정
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    // UART0 초기화 (115200bps)
    BRD  = ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)((BRD - BRDI) * 64 + 0.5);
    UART_init(BRD, BRDI, BRDF);

    // 버저 초기화
    BUZZER_init();
    TimerEn();

    UART_puts("\r\n=== UART Buzzer Scale Test ===\r\n");
    UART_puts("Press 1~7 to play Do Re Mi Fa Sol La Si\r\n");

    while (1) {
        key = UART_getkey();   // 키 입력 대기

        switch (key) {
            case '1':
                UART_puts("Do\r\n");
                Play(N1);
                DelayForPlay(DLY_4);
                break;
            case '2':
                UART_puts("Re\r\n");
                Play(N2);
                DelayForPlay(DLY_4);
                break;
            case '3':
                UART_puts("Mi\r\n");
                Play(N3);
                DelayForPlay(DLY_4);
                break;
            case '4':
                UART_puts("Fa\r\n");
                Play(N4);
                DelayForPlay(DLY_4);
                break;
            case '5':
                UART_puts("Sol\r\n");
                Play(N5);
                DelayForPlay(DLY_4);
                break;
            case '6':
                UART_puts("La\r\n");
                Play(N6);
                DelayForPlay(DLY_4);
                break;
            case '7':
                UART_puts("Si\r\n");
                Play(N7);
                DelayForPlay(DLY_4);
                break;
            default:
                break;
        }
        BUZZEROFF();   // 각 음 끝에 버저 OFF
    }
    return 0;
}

//-----------------------------------------
// Play() : 지정된 주기로 PWM 발생 (50% duty)
//-----------------------------------------
void Play(int freq) {
    BUZZERON();                         // PD4 ON
    GPTMTAILR(TIMER5_BASE) = freq;      // 주기 설정
    GPTMTMATCHR(TIMER5_BASE) = freq/2;  // 듀티비 50%
}

//-----------------------------------------
// DelayForPlay() : WDT 이용한 음 길이 지연
//-----------------------------------------
void DelayForPlay(unsigned int dly) {
    WDT1_LOAD_R = dly;                  // 지연 설정
    while ((WDT1_CTL_R & 0x1) != 0);    // 완료 대기
}
