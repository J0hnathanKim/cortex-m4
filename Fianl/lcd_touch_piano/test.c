#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_RED        0xF800
#define COLOR_ORANGE     0xFC00   // 대충 주황
#define COLOR_YELLOW     0xFFE0
#define COLOR_GREEN      0x07E0
#define COLOR_BLUE       0x001F
#define COLOR_INDIGO     0x481F   // 남색 느낌
#define COLOR_VIOLET     0x781F   // 보라색 느낌
#define COLOR_BLACK      0x0000
#define COLOR_WHITE      0xFFFF

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
uint32_t g_ui32SysClock;

// touch.c 안에 실제로 정의되어 있는 전역 변수
extern int32_t user_X;
extern int32_t user_Y;

// 터치 콜백 프로토타입
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);


void play_note(int idx);



// 0~6 인덱스(도~시)에 해당하는 음 출력
void play_note(int idx) {
    // 도, 레, 미, 파, 솔, 라, 시
    int noteDelay[7] = { 9500, 8500, 7600, 7100, 6300, 5600, 5000 };

    if (idx < 0 || idx > 6) return;

    int d = noteDelay[idx];
    const int noteDuration = 200;
    int j;

    for (j = 0; j < noteDuration; j++) {
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
        delay(d);

        GPIO_WRITE(GPIO_PORTB, 0x04, 0x00);  // buzzer OFF
        delay(d);
    }
    delay(30000); // 짧은 쉼
}

int main(void) {
    float BRD;
    int   BRDI, BRDF;

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);

    // UART (디버깅용)
    BRD  = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);
    Uart_Init(BRD, BRDI, BRDF);

    // LCD
    LCD_Init(g_ui32SysClock);

    // 터치
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

    // BUZZER
    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);   // buzzer enable (보드 회로에 맞게)

    // 배경 지우기
    DrawRect_fill(buffer, 0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);

    // 7개의 키 너비 (480 기준이면 68픽셀씩, 마지막이 약간 더 넓음)
    int keyWidth = LCD_WIDTH / 7;

    // 각 키 색을 배열로 관리
    int colors[7] = {
        COLOR_RED,
        COLOR_ORANGE,
        COLOR_YELLOW,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_INDIGO,
        COLOR_VIOLET
    };

    // 무지개 키 한 번 그려두기
    int i;
    for (i = 0; i < 7; i++) {
        int x1 = i * keyWidth;
        int x2 = (i == 6) ? (LCD_WIDTH - 1) : ((i + 1) * keyWidth - 1);
        DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, colors[i]);
    }

    // 터치 초기값
    user_X = -1;
    user_Y = -1;

    while (1) {
        // 터치가 유효 범위 안에 있을 때만 처리
        if (user_X >= 0 && user_X < LCD_WIDTH &&
            user_Y >= 0 && user_Y < LCD_HEIGHT) {

            int keyWidth = LCD_WIDTH / 7;
            int idx = user_X / keyWidth;   // 0~6
            if (idx > 6) idx = 6;

            // 디버깅용 출력
            Uart_Printf("Touch x=%d y=%d -> key=%d\n\r", user_X, user_Y, idx);

            // 해당 키에 맞는 음 출력
            play_note(idx);

            // 같은 터치로 여러 번 울리는 것 방지
            user_X = -1;
            user_Y = -1;
        }

        delay(10000);
    }

    return 0;
}

// 터치 콜백: 좌표만 저장
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
    user_X = i32X;
    user_Y = i32Y;

    // 필요 없으면 주석 처리 가능
    // Uart_Printf("Raw Touch x:%d y:%d\n\r", user_X, user_Y);

    return 0;
}
