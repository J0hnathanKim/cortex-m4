#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE		0x001f
#define COLOR_GREEN		0x07E0
#define COLOR_RED		0xF800
#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000
#define Image00			0x00000
#define Image01			0x40000
#define UART0_FR_R              (*((volatile uint32_t *)(UART0_BASE + 0x018)))
#define UART_FR_RXFE            0x00000010  // Receive FIFO Empty

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];

uint32_t g_ui32SysClock;
void delay_ms(int ms) {
    volatile int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 4000; j++) {
        }
    }
}

void jump_parabola(int *x1, int *y1, int *x2, int *y2) {
    const int dx = 10;   // 한 스텝당 오른쪽 이동
    const int dy = 10;   // 한 스텝당 위/아래 이동
    int i;

    // 1) 올라가는 구간 (예: 4스텝)
    for (i = 0; i < 5; i++) {
        // 현재 위치 지우기 (배경색으로 덮기)
        DrawRect(*x1, *y1, *x2, *y2, COLOR_GREEN);

        // 위치 업데이트
        *x1 += dx;
        *x2 += dx;
        *y1 += dy;   // 위로 (너의 좌표계 기준)
        *y2 += dy;

        // 새 위치 그리기
        DrawRect(*x1, *y1, *x2, *y2, COLOR_RED);


    }

    // 2) 내려가는 구간 (예: 4스텝)
    for (i = 0; i < 5; i++) {
        DrawRect(*x1, *y1, *x2, *y2, COLOR_GREEN);

        *x1 += dx;
        *x2 += dx;
        *y1 -= dy;   // 아래로
        *y2 -= dy;

        DrawRect(*x1, *y1, *x2, *y2, COLOR_RED);


    }
}


int main(void) {
	uint8_t code;

	float BRD;
	int   BRDI;
	int   BRDF;
	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);


	LCD_Init(g_ui32SysClock);
    int x1, x2, y1, y2;
    x1 = 50;
    x2 = 70;
    y1 = 100;
    y2 = 120;
    int rx1 = 460;
    int ry1 = 100;
    int rx2 = 480;
    int ry2 = 120;



	BRD = (float)g_ui32SysClock / (16 * 115200);
	BRDI = (int)BRD;
	BRDF = (int)(((BRD - (int)BRD)*64) + 0.5);
    BUZZER_init();
    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);
    int noteDelay[] = {
          9500,  // 도
          8500,  // 레
          7600 //미
      };
    int i, j;
    const int numNotes = 3;
    const int noteDuration = 200;
	UART_init(BRD, BRDI, BRDF);


	UART_printf("//**************************************//\n\r");
	UART_printf("//***********  Initialize  *************//\n\r");
	UART_printf("//***********      OK      *************//\n\r");
	UART_printf("//**************************************//\n\r");
	DrawScaledImage(buffer, 0, 0, 240, 100, Image01);
	DrawScaledImage(buffer, 240, 0, 480, 100, Image00);
	while(1){
		DrawRect(x1, y1, x2, y2, COLOR_RED);
		DrawRect(rx1, ry1, rx2, ry2, COLOR_BLUE);
	    Scroll_left(buffer, 0, 0, 480, 100, 10);
	    // 파란 네모를 왼쪽으로 이동
	   DrawRect(rx1, ry1, rx2, ry2, COLOR_GREEN);  // 이전 위치 지우기
	        rx1 -= 10;  // 왼쪽으로 1픽셀 이동 (스크롤 속도에 맞춰 조정)
	        rx2 -= 10;

	        // 화면 왼쪽 끝을 벗어나면 오른쪽에서 다시 등장
	        if(rx1 < 0) {
	            rx1 = 460;
	            rx2 = 480;
	        }

	    DrawRect(rx1, ry1, rx2, ry2, COLOR_BLUE);  // 새 위치에 그리기
	    // UART 수신 FIFO가 비어있지 않으면 (데이터가 있으면)
	    if((UART0_FR_R & UART_FR_RXFE) == 0) {
	        code = UART_getch();

	        if(code == 'd'){
	            DrawRect(x1, y1, x2, y2, COLOR_GREEN);
	            x1 += 10;
	            x2 += 10;
	            DrawRect(x1, y1, x2, y2, COLOR_RED);
	        }
	        else if(code == 'a'){
	            DrawRect(x1, y1, x2, y2, COLOR_GREEN);
	            x1 -= 10;
	            x2 -= 10;
	            DrawRect(x1, y1, x2, y2, COLOR_RED);
	        }
	        else if (code == 'j') {
	            jump_parabola(&x1, &y1, &x2, &y2);
	        }
	    }
	    if (x1 < rx2 && x2 > rx1 && y1 < ry2 && y2 > ry1) {
	    	 for (i = 0; i < numNotes; i++) {
	    	              int d = noteDelay[i];

	    	              // 하나의 음을 일정 시간동안 유지
	    	              for (j = 0; j < noteDuration; j++) {
	    	                  GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);   // buzzer ON (출력 HIGH)
	    	                  delay(d);

	    	                  GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buzzer OFF (출력 LOW)
	    	                  delay(d);
	    	              }

	    	              // 음과 음 사이에 약간의 공백(쉼표 느낌)
	    	              delay(30000);
	    	 }
	    }

	}
}
