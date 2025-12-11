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
    uint8_t code = 0;
    float BRD;
    int   BRDI, BRDF;

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                         SYSCTL_CFG_VCO_480), 120000000);



    FND_init();
    FND_clear();

    // UART
    BRD  = (float)g_ui32SysClock / (16 * 115200);
    BRDI = (int)BRD;
    BRDF = (int)(((BRD - BRDI) * 64) + 0.5);
    Uart_Init(BRD, BRDI, BRDF);

    // LCD
    LCD_Init(g_ui32SysClock);

    // Touch (지금은 안 써도 됨)
    TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

    // BUZZER
    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);   // buzzer enable

    // 배경
    DrawRect_fill(buffer, 0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_BLACK);

    int keyWidth = LCD_WIDTH / 7;

    int colors[7] = {
        COLOR_RED,      // 1: 도
        COLOR_ORANGE,   // 2: 레
        COLOR_YELLOW,   // 3: 미
        COLOR_GREEN,    // 4: 파
        COLOR_BLUE,     // 5: 솔
        COLOR_INDIGO,   // 6: 라
        COLOR_VIOLET    // 7: 시
    };

    // 무지개 키 그리기
    int i;
    for(i = 0; i < 7; i++){
        int x1 = i * keyWidth;
        int x2 = (i == 6) ? (LCD_WIDTH - 1) : ((i + 1) * keyWidth - 1);
        DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, colors[i]);
    }

    user_X = -1;
    user_Y = -1;
    int flag = 0;
    Uart_Printf("//**************************************//\n\r");
    Uart_Printf("//***********  Initialize  *************//\n\r");
    Uart_Printf("//***********  Piano Ready  ************//\n\r");
    Uart_Printf("//**************************************//\n\r");
    while(1){

        // 1단계: FND 모드 (UART 들어올 때까지 3만 표시)

            WRITE_FND(3, code - '0');

            // UART에 데이터 들어왔는지 검사 (논블로킹)
            if ((UART0_FR_R & UART_FR_RXFE) == 0) {   // FIFO가 비어있지 않다면
                code = Uart_GetKey();                 // 한 글자 읽고
                if(code >= '1' && code <= '7') {
                            int idx = code - '1';

                            int x1 = idx * keyWidth;
                            int x2 = (idx == 6) ? (LCD_WIDTH - 1) : ((idx + 1) * keyWidth - 1);

                            DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, COLOR_WHITE);
                            play_note(idx);
                            DrawRect_fill(buffer, x1, 0, x2, LCD_HEIGHT - 1, colors[idx]);
                        }

                        delay(10000);

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
