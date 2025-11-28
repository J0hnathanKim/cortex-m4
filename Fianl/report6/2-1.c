#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE       0x001F
#define COLOR_GREEN      0x07E0
#define COLOR_RED        0xF800
#define COLOR_WHITE      0xFFFF
#define COLOR_BLACK      0x0000

// 중앙 100x100 사각형 좌표 (480x272 기준 거의 중앙)
#define RECT_X1          190
#define RECT_Y1          86      // 272-100 = 172, 172/2 = 86
#define RECT_X2          (RECT_X1 + 100)
#define RECT_Y2          (RECT_Y1 + 100)

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
uint32_t g_ui32SysClock;

// 터치 좌표를 저장할 전역 변수
extern int32_t user_X;
extern int32_t user_Y;


// 사각형 상태
// 0: 사각형 없음
// 1: 빨간색
// 2: 파란색
// 3: 초록색
int rect_state = 0;

// 함수 프로토타입
void delay(int count);
void play_do(void);
void play_si(void);
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);

// -------------------- 딜레이 함수 --------------------
void delay(int count) {
    volatile int i;
    for (i = 0; i < count; i++) {
        // busy-wait
    }
}

// -------------------- '도' 출력 --------------------
void play_do(void) {
    int d = 9500;          // 도
    const int noteDuration = 200;
    int j;

    for (j = 0; j < noteDuration; j++) {
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
        delay(d);

        GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buzzer OFF
        delay(d);
    }
}

// -------------------- '시' 출력 --------------------
void play_si(void) {
    int d = 5000;          // 시 (도레미파솔라시도에서 7번째)
    const int noteDuration = 200;
    int j;

    for (j = 0; j < noteDuration; j++) {
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);  // buzzer ON
        delay(d);

        GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buz저 OFF
        delay(d);
    }
}

// -------------------- 터치 콜백 --------------------
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y) {
    // 터치 좌표를 전역 변수에 저장
    user_X = i32X;
    user_Y = i32Y;

    // 디버깅용 UART 출력 (원하면 주석 처리해도 됨)
    if (user_X >= 0 && user_X < 480 && user_Y >= 0 && user_Y < 272) {
        Uart_Printf("Touch : x = %d, y = %d\n\r", user_X, user_Y);
    }

    return 0;
}

int main(void) {
    int push1_current, push2_current;
    int push1_prev, push2_prev;
    user_X = -1;
    user_Y = -1;

    float BRD;
    int   BRDI;
    int   BRDF;

    // 시스템 클럭 설정
    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);

    // BUZZER 초기화
    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer enable (보드에 따라 다를 수 있음)

    // PUSH 스위치 초기화
    PUSH_init();

    // UART 설정 (디버깅용)
    BRD  = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);
    Uart_Init(BRD, BRDI, BRDF);

    // LCD 초기화
    LCD_Init(g_ui32SysClock);

    // 터치 초기화
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

    // 초기 화면 지우기 (배경색: 검정)
    DrawRect_fill(buffer, 0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);

    // 스위치 초기값 읽기 (엣지 검출용)
    push1_prev = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
    push2_prev = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2

    while (1) {
        // 1) 현재 상태에 맞는 사각형 그리기
        if (rect_state == 0) {
            // 사각형 없음 -> 해당 영역을 배경색으로 지움
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_BLACK);
        } else if (rect_state == 1) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_RED);
        } else if (rect_state == 2) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_BLUE);
        } else if (rect_state == 3) {
            DrawRect_fill(buffer, RECT_X1, RECT_Y1, RECT_X2, RECT_Y2, COLOR_GREEN);
        }

        // 2) 스위치 상태 읽기
        push1_current = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
        push2_current = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2

        // 약간의 디바운싱 딜레이
        delay(10000);

        // 3) PUSH_SW 1: 사각형 생성 (빨간색)
        //    high -> low (1 -> 0) 순간을 눌림으로 판단 (풀업 스위치 기준)
        if (push1_prev != 0 && push1_current == 0) {
            if (rect_state == 0) {
                rect_state = 1;  // 빨간 사각형 등장
            }
        }

        // 4) PUSH_SW 2: 초록 사각형 상태에서 사각형 삭제
        if (push2_prev != 0 && push2_current == 0) {
            if (rect_state == 3) {
                rect_state = 0;  // 사각형 제거
            }
        }

        // 5) 터치 처리
        //    - 밖을 터치하면 아무 변화 없음
        //    - 안을 터치하면 상태 변화 + 음계 출력
        if (user_X >= RECT_X1 && user_X <= RECT_X2 &&
            user_Y >= RECT_Y1 && user_Y <= RECT_Y2) {

            if (rect_state == 1) {
                // 빨강 -> 파랑 + '도'
                rect_state = 2;
                play_do();
                // 같은 터치로 여러 번 트리거되는 것 방지
                user_X = -1;
                user_Y = -1;
            } else if (rect_state == 2) {
                // 파랑 -> 초록 + '시'
                rect_state = 3;
                play_si();
                user_X = -1;
                user_Y = -1;
            }
            // 초록 상태에서는 터치해도 색 변화/음계 없음 (과제 조건엔 없음)
        }

        // 6) 다음 루프를 위한 스위치 상태 저장
        push1_prev = push1_current;
        push2_prev = push2_current;

        // 루프 딜레이 (너무 빠르게 돌지 않게)
        delay(10000);
    }

    return 0;
}
