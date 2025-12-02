#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_RED        0xF800
#define COLOR_ORANGE     0xFC00
#define COLOR_YELLOW     0xFFE0
#define COLOR_GREEN      0x07E0
#define COLOR_BLUE       0x001F
#define COLOR_INDIGO     0x481F
#define COLOR_VIOLET     0x781F
#define COLOR_WHITE      0xFFFF
#define COLOR_BLACK      0x0000

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
uint32_t g_ui32SysClock;

extern int32_t user_X;
extern int32_t user_Y;
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);

void delay(int count){
    volatile int i;
    for(i=0;i<count;i++);
}

// 음계 출력
void play_note(int idx) {
    int noteDelay[7] = { 9500, 8500, 7600, 7100, 6300, 5600, 5000 };
    int d = noteDelay[idx];
    const int noteDuration = 200;

    for(int j = 0; j < noteDuration; j++){
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);
        delay(d);
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x00);
        delay(d);
    }
    delay(30000);
}

int main(void) {
    float BRD;
    int   BRDI, BRDF;

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);

    // UART
    BRD  = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);
    Uart_Init(BRD, BRDI, BRDF);

    // LCD
    LCD_Init(g_ui32SysClock);

    // Touch
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

    // BUZZER
    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);

    // 배경
    DrawRect_fill(buffer, 0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);

    int keyWidth = LCD_WIDTH / 7;

    int colors[7] = {
        COLOR_RED,
        COLOR_ORANGE,
        COLOR_YELLOW,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_INDIGO,
        COLOR_VIOLET
    };

    // 무지개 키 그리기
    for(int i = 0; i < 7; i++){
        int x1 = i * keyWidth;
        int x2 = (i == 6) ? (LCD_WIDTH - 1) : ((i + 1)*keyWidth - 1);
        DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, colors[i]);
    }

    user_X = -1;
    user_Y = -1;

    while(1){

        if(user_X >= 0 && user_X < LCD_WIDTH &&
           user_Y >= 0 && user_Y < LCD_HEIGHT){

            int idx = user_X / keyWidth;
            if(idx > 6) idx = 6;

            // 건반 영역 계산
            int x1 = idx * keyWidth;
            int x2 = (idx == 6) ? (LCD_WIDTH - 1) : ((idx + 1)*keyWidth - 1);

            // 1) 눌린 건반을 하얀색으로 바꿈
            DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, COLOR_WHITE);

            // 2) 음 재생
            play_note(idx);

            // 3) 원래 색으로 복원
            DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, colors[idx]);

            // 중복 재생 방지
            user_X = -1;
            user_Y = -1;
        }

        delay(10000);
    }

    return 0;
}

// 터치 콜백
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
    user_X = i32X;
    user_Y = i32Y;
    return 0;
}
