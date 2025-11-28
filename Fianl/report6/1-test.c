#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE      0x001f
#define COLOR_GREEN     0x07E0
#define COLOR_RED       0xF800
#define COLOR_WHITE     0xFFFF
#define COLOR_BLACK     0x0000
#define Image00         0x00000
#define Image01         0x40000
#define UART0_FR_R      (*((volatile uint32_t *)(UART0_BASE + 0x018)))
#define UART_FR_RXFE    0x00000010  // Receive FIFO Empty

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];

uint32_t g_ui32SysClock;

/* ---- 버저용 딜레이 ---- */
void buzz_delay(int count) {
    volatile int i;
    for (i = 0; i < count; i++) { }
}

/* ---- 도레미 재생 ---- */
void play_doremi(void) {
    int noteDelay[] = { 9500, 8500, 7600 }; // 도,레,미
    int numNotes = 3;
    int noteDuration = 150;
    int i;
    for (i = 0; i < numNotes; i++) {
        int d = noteDelay[i];
        int j;
        for (j = 0; j < noteDuration; j++) {
            GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);   // buzzer ON
            buzz_delay(d);

            GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buzzer OFF
            buzz_delay(d);
        }

        buzz_delay(30000); // 쉼표
    }
}

/* ---- 점프 ---- */
void jump_parabola(int *x1, int *y1, int *x2, int *y2) {
    const int dx = 10;
    const int dy = 10;
    int i;
    for (i = 0; i < 5; i++) {
        DrawRect(*x1, *y1, *x2, *y2, COLOR_GREEN);
        *x1 += dx; *x2 += dx;
        *y1 += dy; *y2 += dy;
        DrawRect(*x1, *y1, *x2, *y2, COLOR_RED);
    }

    for (i = 0; i < 5; i++) {
        DrawRect(*x1, *y1, *x2, *y2, COLOR_GREEN);
        *x1 += dx; *x2 += dx;
        *y1 -= dy; *y2 -= dy;
        DrawRect(*x1, *y1, *x2, *y2, COLOR_RED);
    }
}

int main(void) {
    uint8_t code;

    float BRD;
    int   BRDI, BRDF;

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);

    // *** TIMER_init(); 제거 ***
    // *** GPTMCTL enable 제거 ***

    BUZZER_init();
    WDTinitISR();

    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer enable only
    BUZZER_clear();

    LCD_Init(g_ui32SysClock);

    int x1 = 50,  x2 = 70;
    int y1 = 100, y2 = 120;

    int rx1 = 460, rx2 = 480;
    int ry1 = 100, ry2 = 120;

    int collided = 0; // 충돌 플래그

    // UART 설정
    BRD = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);

    UART_init(BRD, BRDI, BRDF);

    UART_printf("//***********  Initialize  *************//\n\r");

    DrawScaledImage(buffer, 0,   0, 240, 100, Image01);
    DrawScaledImage(buffer, 240, 0, 480, 100, Image00);

    while (1) {
        DrawRect(x1, y1, x2, y2, COLOR_RED);
        DrawRect(rx1, ry1, rx2, ry2, COLOR_BLUE);

        // 상단 이미지 스크롤
        Scroll_left(buffer, 0, 0, 480, 100, 10);

        // 파란 사각형 이동
        DrawRect(rx1, ry1, rx2, ry2, COLOR_GREEN);
        rx1 -= 10; rx2 -= 10;

        if (rx1 < 0) {  // 화면 왼쪽 넘어가면 리셋
            rx1 = 460; rx2 = 480;
        }

        DrawRect(rx1, ry1, rx2, ry2, COLOR_BLUE);

        // ---- AABB 충돌 여부 검사 ----
        int touching =
            (x1 < rx2) && (x2 > rx1) &&
            (y1 < ry2) && (y2 > ry1);

        if (touching) {
            if (!collided) {
                play_doremi();   // 처음 닿은 순간만 재생
                collided = 1;
            }
        } else {
            collided = 0;  // 떨어지면 재생 가능 상태로 초기화
        }

        // ---- UART 입력 ----
        if ((UART0_FR_R & UART_FR_RXFE) == 0) {
            code = UART_getch();

            if (code == 'd') {
                DrawRect(x1, y1, x2, y2, COLOR_GREEN);
                x1 += 10; x2 += 10;
                DrawRect(x1, y1, x2, y2, COLOR_RED);
            }
            else if (code == 'a') {
                DrawRect(x1, y1, x2, y2, COLOR_GREEN);
                x1 -= 10; x2 -= 10;
                DrawRect(x1, y1, x2, y2, COLOR_RED);
            }
            else if (code == 'j') {
                jump_parabola(&x1, &y1, &x2, &y2);
            }
        }
    }

    return 0;
}
