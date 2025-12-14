#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_RED        0xF800
#define COLOR_ORANGE     0xFC00
#define COLOR_YELLOW     0xFFE0
#define COLOR_GREEN      0x07E0
#define COLOR_BLUE       0x001F
#define COLOR_INDIGO     0x081F
#define COLOR_VIOLET     0x901F
#define COLOR_WHITE      0xFFFF
#define COLOR_BLACK      0x0000
#define UART0_FR_R              (*((volatile uint32_t *)(UART0_BASE + 0x018)))
#define UART_FR_RXFE            0x00000010  // Receive FIFO Empty

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
uint32_t g_ui32SysClock;

// (터치는 지금 안 쓰지만 있어도 문제 없음)
extern int32_t user_X;
extern int32_t user_Y;
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);

void delay(int count){
    volatile int i;
    for(i=0;i<count;i++);
}

// idx: 0~6 → 도,레,미,파,솔,라,시
void play_note(int idx) {
    int noteDelay[7] = { 9500, 8500, 7600, 7100, 6300, 5600, 5000 };
    int d = noteDelay[idx];
    const int noteDuration = 200;
    int j;

    for(j = 0; j < noteDuration; j++){
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x04); // buzzer ON
        delay(d);
        GPIO_WRITE(GPIO_PORTB, 0x04, 0x00); // buzzer OFF
        delay(d);
    }
    delay(30000);
}

int main(void) {
    int push2_current;
    int push2_prev = 0;
    uint8_t code = 0;
    float BRD;
    int BRDI, BRDF;

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);

    PUSH_init();
    FND_init();
    FND_clear();
    LED_init();
    LED_clear();

    BRD = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);
    Uart_Init(BRD, BRDI, BRDF);

    LCD_Init(g_ui32SysClock);
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);

    // 배경
    DrawRect_fill(buffer, 0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);

    int led = 0b100000000;
    int x1 = 430;
    int y1 = 30;
    int x2 = 480;
    int y2 = 80;
    int color = COLOR_RED;
    int move = 0;
    int fnd = 0;

    user_X = -1;
    user_Y = -1;

    // 초기 사각형 그리기
    DrawRect_fill(buffer, x1, y1, x2, y2, color);

    Uart_Printf("//**************************************//\n\r");
    Uart_Printf("//***********  Initialize  *************//\n\r");
    Uart_Printf("//***********  Ready  ************//\n\r");
    Uart_Printf("//**************************************//\n\r");

    while(1){
        // 1) 스위치 읽기
        push2_current = GPIO_READ(GPIO_PORTN, PIN3);

        // 2) 스위치 눌림(엣지) 감지
        if (push2_prev != 0 && push2_current == 0) {
            color = COLOR_BLUE;
            DrawRect_fill(buffer, x1, y1, x2, y2, color);
        }
        push2_prev = push2_current;

        // 3) FND 표시
        WRITE_FND(6, fnd);

        // 4) UART 입력 검사
        if ((UART0_FR_R & UART_FR_RXFE) == 0) {
            code = Uart_GetKey();
            switch(code){
                case 'a':
                    DrawRect_fill(buffer, x1, y1, x2, y2, COLOR_BLACK); // 이전 위치 지우기
                    x1 -= 10;
                    x2 -= 10;
                    move += 10;
                    DrawRect_fill(buffer, x1, y1, x2, y2, color); // 새 위치에 그리기
                    break;
            }
        }

        // 5) 50픽셀마다 음 재생 및 LED
        if(move % 50 == 0 && move >= 50) {
            fnd += 1;
            play_note(0);
            led = (led >> 1);
            GPIO_WRITE(GPIO_PORTL, 0xf, led & 0xf);
            GPIO_WRITE(GPIO_PORTM, 0xf, (led>>4)&0xf);

            move = 0; // move를 리셋해야 중복 실행 방지
        }
    }
    return 0;
}

// 터치 콜백 (지금은 좌표만 저장)
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
    user_X = i32X;
    user_Y = i32Y;
    return 0;
}

void LED_clear(){
	GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
	GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
	delay(2500000);
}
