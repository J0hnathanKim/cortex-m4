#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE		0x001f
#define COLOR_GREEN		0x07E0
#define COLOR_RED		0xF800
#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000
#define COLOR_DARK_GREEN   0x03E0

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
	int push1_current, push2_current;
	int push1_prev = 0, push2_prev = 0;
    int color = COLOR_GREEN;
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

	PUSH_init();
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
		DrawRect_fill(buffer, 190, 120, 290, 220, color);
		push1_current = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
		push2_current = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2
		delay(10000);
		if(push1_prev != 0 && push1_current == 0) {
			color = COLOR_RED;
		}
		if(push2_prev != 0 && push2_current == 0) {
			color = COLOR_GREEN;
		}

		if((user_X >= 190 && user_X <=290)&&(user_Y >= 120 && user_Y <= 220)){
			if(color == COLOR_RED) {color = COLOR_BLUE;
			user_X = 0;
			user_Y = 0;
			delay(10000000);
			}
		}

	    if(color == COLOR_BLUE){
			if((user_X >= 190 && user_X <=290)&&(user_Y >= 120 && user_Y <= 220)) color = COLOR_DARK_GREEN;
		}
	    push1_prev = push1_current;
	    push2_prev = push2_current;
	    delay(10000);

	}

	return 0;
}

int32_t PointerMessage(uint32_t ui32Message, int32_t i32X, int32_t i32Y){
    if(user_X >= 0 && user_X < 480 && user_Y >= 0 && user_Y < 272){
    	Uart_Printf("TEST 0 x : %d, y : %d \n\r", user_X, user_Y);

    }
	return 0;
}
