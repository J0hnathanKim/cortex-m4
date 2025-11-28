#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE       0x001F
#define COLOR_GREEN      0x07E0
#define COLOR_RED        0xF800
#define COLOR_WHITE      0xFFFF
#define COLOR_BLACK      0x0000

// 중앙 100x100 사각형 위치 (480x272 기준)
#define RECT_X1          190
#define RECT_Y1          86
#define RECT_X2          (RECT_X1 + 100)
#define RECT_Y2          (RECT_Y1 + 100)

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
uint32_t g_ui32SysClock;

// touch.c 에서 실제로 정의되어 있는 전역 변수들
extern int32_t user_X;
extern int32_t user_Y;

// touch.c 안에 PointerMessage가 정의되어 있다고 가정
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);

// 사각형 상태
// 0: 없음, 1: 빨강, 2: 파랑, 3: 초록
int rect_state = 0;

void delay(int count);
void play_do(void);
void play_si(void);

// -------------------- 딜레이 --------------------
void delay(int count) {
    volatile int i;
    for (i = 0; i < count; i++) {
        // busy wait
    }
}

// -------------------- '도' 출력 --------------------
void play_do(void) {
    int d = 9500;             // 도
    const int noteDuration = 200;
    int j;

    for (j = 0; j < noteDuration; j++) {
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
        delay(d);

        GPIO_WRITE(GPIO_PORTB, 0x04, 0x00);  // buzzer OFF
        delay(d);
    }

    // 짧은 쉼표
    delay(30000);
}

// -------------------- '시' 출력 --------------------
void play_si(void) {
    int d = 5000;             // 시
    const int noteDuration = 200;
    int j;

    for (j = 0; j < noteDuration; j++) {
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
        delay(d);

        GPIO_WRITE(GPIO_PORTB, 0x04, 0x00);  // buzzer OFF
        delay(d);
    }

    delay(30000);
}

int main(void) {
    int push1_current, push2_current;
    int push1_prev, push2_prev;

    float BRD;
    int   BRDI;
    int   BRDF;

    // 1) 시스템 클럭
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);

    // 2) UART (디버깅용)
    BRD  = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);
    Uart_Init(BRD, BRDI, BRDF);

    // 3) LCD
    LCD_Init(g_ui32SysClock);

    // 4) 터치
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);  // touch.c 안의 콜백 사용

    // 5) PUSH 스위치
    PUSH_init();

    // *** 여기까지가 나머지 주변장치 초기화 ***

    // 6) BUZZER를 마지막에 다시 초기화해서, 다른 모듈이 덮어쓴 GPIO 설정을 복구
    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);   // buzzer enable (보드 회로도 기준)

    // 배경 초기화
    DrawRect_fill(buffer, 0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);

    // 스위치 초기값 읽기 (엣지 검출용)
    push1_prev = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
    push2_prev = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2

    // 터치 초기값 (안전하게 사각형 밖으로)
    user_X = -1;
    user_Y = -1;

    while (1) {

        // 1) 현재 사각형 상태에 따른 색 그리기
        if (rect_state == 0) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_BLACK);
        } else if (rect_state == 1) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_RED);
        } else if (rect_state == 2) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_BLUE);
        } else if (rect_state == 3) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_GREEN);
        }

        // 2) 스위치 현재 값 읽기
        push1_current = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
        push2_current = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2

        // 디바운싱 조금
        delay(10000);

        // 3) PUSH_SW 1 눌림: 사각형 생성 (빨강)
        //    high -> low (1 -> 0)을 눌림으로 처리 (풀업 스위치 가정)
        if (push1_prev != 0 && push1_current == 0) {
            if (rect_state == 0) {
                rect_state = 1;    // 빨간 사각형 등장
            }
        }

        // 4) PUSH_SW 2 눌림: 초록 상태에서 사각형 제거
        if (push2_prev != 0 && push2_current == 0) {
            if (rect_state == 3) {
                rect_state = 0;    // 사각형 제거
            }
        }

        // 5) 터치 처리
        //    빨강/파랑일 때, 사각형 안을 터치해야만 색 + 음계 변화
        if (user_X >= RECT_X1 && user_X <= RECT_X2 &&
            user_Y >= RECT_Y1 && user_Y <= RECT_Y2) {

            if (rect_state == 1) {
                // 빨강 -> 파랑 + '도'
                rect_state = 2;
                play_do();
                user_X = -1;   // 같은 터치로 여러 번 트리거되는 것 방지
                user_Y = -1;
            } else if (rect_state == 2) {
                // 파랑 -> 초록 + '시'
                rect_state = 3;
                play_si();
                user_X = -1;
                user_Y = -1;
            }
        }

        // 6) 다음 루프 대비: 이전 스위치 값 갱신
        push1_prev = push1_current;
        push2_prev = push2_current;

        delay(10000);
    }

    return 0;
}

int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
    user_X = i32X;
    user_Y = i32Y;

    Uart_Printf("Touch x:%d y:%d\n\r", user_X, user_Y);
    return 0;
}

