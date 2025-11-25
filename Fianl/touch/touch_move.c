#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE		0x001f
#define COLOR_GREEN		0x07E0
#define COLOR_RED		0xF800
#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000

#define image1			0x000000

//volatile unsigned int variable0 = 0;

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y);
extern int32_t user_X, user_Y;
uint32_t g_ui32SysClock;

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];
void delay(int count){
	while(count != 0){
		count--;
	}
}

int main(void) {

	float BRD;
	int   BRDI;
	int   BRDF;
    int x1 = 50;
    int y1 = 80;
    int x2 = 70;
    int y2 =100;
	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
										SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
										SYSCTL_CFG_VCO_480), 120000000);

	BRD = (float)g_ui32SysClock / (16 * 115200);
	BRDI = (int)BRD;
	BRDF = (int)(((BRD - (int)BRD)*64) + 0.5);

	//Uart
	Uart_Init(BRD, BRDI, BRDF);
	//LCD
	LCD_Init(g_ui32SysClock);
	//Touch
	TouchScreenInit(g_ui32SysClock);
    TouchScreenCallbackSet(PointerMessage);

	while (1){
		delay(10000);
		DrawRect_fill(buffer, x1, y1, x2, y2, COLOR_RED);
		if(x1 != user_X && y1 != user_Y){
		DrawRect_fill(buffer, x1, y1, x2, y2, COLOR_GREEN);
		x1 = user_X;
		y1 = user_Y;
		x2 = user_X + 30;
		y2 = user_Y + 30;
		DrawRect_fill(buffer, x1, y1, x2, y2, COLOR_RED);
		}
	}

	return 0;
}

int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
    if(user_X >= 0 && user_X < 480 && user_Y >= 0 && user_Y < 272){
    	Uart_Printf("TEST 0 x : %d, y : %d \n\r", user_X, user_Y);

    }
	return 0;
}
