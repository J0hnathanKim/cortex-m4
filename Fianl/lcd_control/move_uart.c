#include "cortex_m4.h"
#include "myLib.h"
#include "math.h"
#include "time.h"

#define COLOR_BLUE		0x001f
#define COLOR_GREEN		0x07E0
#define COLOR_RED		0xF800
#define COLOR_WHITE		0xFFFF
#define COLOR_BLACK		0x0000

unsigned char buffer[LCD_WIDTH * LCD_HEIGHT];

uint32_t g_ui32SysClock;

int main(void) {
	uint8_t code;

	float BRD;
	int   BRDI;
	int   BRDF;
	g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);


	LCD_Init(g_ui32SysClock);
    int x1, x2, y1, y2;
    x1 = 50;
    x2 = 100;
    y1 = 70;
    y2 = 120;

	DrawRect(x1, y1, x2, y2, COLOR_BLUE);


	BRD = (float)g_ui32SysClock / (16 * 115200);
	BRDI = (int)BRD;
	BRDF = (int)(((BRD - (int)BRD)*64) + 0.5);

	UART_init(BRD, BRDI, BRDF);


	UART_printf("//**************************************//\n\r");
	UART_printf("//***********  Initialize  *************//\n\r");
	UART_printf("//***********      OK      *************//\n\r");
	UART_printf("//**************************************//\n\r");

	while(1){
		code = UART_getch();
		if(code == 'd'){
			DrawRect(x1, y1, x2, y2, COLOR_GREEN);
			x1 += 10;
			x2 += 10;
			DrawRect(x1, y1, x2, y2, COLOR_BLUE);
		}
		else if(code == 'a'){
			DrawRect(x1, y1, x2, y2, COLOR_GREEN);
			x1 -= 10;
			x2 -= 10;
			DrawRect(x1, y1, x2, y2, COLOR_BLUE);
		}

	}


}

