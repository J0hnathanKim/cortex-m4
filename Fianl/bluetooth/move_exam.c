/*
 * main.c
 */

#include "cortex_m4.h"
#include "myLib.h"
#define COLOR_BLUE		0x001f
#define COLOR_GREEN		0x07E0

char Uart_GetCh(void);
char Uart_GetKey(void);
void Uart_PutCh(uint8_t data);
void Uart_PutStr(char* pt);
void Uart_Printf(char *fmt,...);

char Bluetooth_GetCh(void);
char Bluetooth_GetKey(void);
void Bluetooth_PutCh(uint8_t data);
void Bluetooth_PutStr(char* pt);
void Bluetooth_Printf(char *fmt,...);

void _user_Bluetooth_Interrupt_Handler(void);

int main(void) {
	float BRD;
	int BRDI;
	int BRDF;

	char code = 0;

	uint32_t ui32SysClock;

	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
	SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
	SYSCTL_CFG_VCO_480), 120000000);

	//uart baud rate(p.1339)
	BRD = (float) ui32SysClock / (16 * 115200);
	BRDI = (int) BRD;
	BRDF = (int) (((BRD - (int) BRD) * 64) + 0.5);
	UART_init(BRD, BRDI, BRDF);
	Bluetooth_init(BRD, BRDI, BRDF);

	Uart_Printf("//**************************************//\n\r");
	Uart_Printf("//***********  Initialize  *************//\n\r");
	LCD_Init(ui32SysClock);
	int x1, x2, y1, y2;
	    x1 = 50;
	    x2 = 100;
	    y1 = 70;
	    y2 = 120;

		DrawRect(x1, y1, x2, y2, COLOR_BLUE);


	while (1) {
		code = Bluetooth_GetKey();
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

	return 0;
}

void _user_Bluetooth_Interrupt_Handler(void) {
	char Bluetooth_Data;

	UART3IM = UART3IM & ~(0x1 << 4);

	Bluetooth_Data = Bluetooth_GetKey();	// 외부 장치로부터 받은 데이터를 저장
	Uart_PutCh(Bluetooth_Data);				// 터미널에 해당 데이터 출력

	UART3IM = UART3IM | (0x1 << 4);
	UART3ICR = UART3ICR | (0x1 << 4);
	INTUNPEND1 = INTPEND1;
}


char Uart_GetCh(void) {
	while (!(UART0FR & 0x40));
	return UART0_DATA;
}

char Uart_GetKey(void) {
	if (!(UART0FR & 0x40)) {
		return 0;
	} else
		return UART0_DATA;
}

void Uart_PutCh(uint8_t data) {
	UART0_DATA = data;
	while (!(UART0FR & 0x80));
}


void Uart_PutStr(char* pt) {
	while (*pt)
		Uart_PutCh(*pt++);
}

void Uart_Printf(char *fmt, ...) {
	va_list ap;
	char ch_buffer[256];

	va_start(ap, fmt);
	vsprintf(ch_buffer, (const char*) fmt, ap);

	Uart_PutStr(ch_buffer);
	va_end(ap);
}

char Bluetooth_GetCh(void) {
	while (!(UART3FR & 0x40));
	return UART3_DATA;
}

char Bluetooth_GetKey(void) {
	if (!(UART3FR & 0x40)) {
		return 0;
	} else
		return UART3_DATA;
}

void Bluetooth_PutCh(uint8_t data) {
	UART3_DATA = data;
	while (!(UART3FR & 0x80));
}


void Bluetooth_PutStr(char* pt) {
	while (*pt)
		Uart_PutCh(*pt++);
}

void Bluetooth_Printf(char *fmt, ...) {
	va_list ap;
	char ch_buffer[256];

	va_start(ap, fmt);
	vsprintf(ch_buffer, (const char*) fmt, ap);

	Uart_PutStr(ch_buffer);
	va_end(ap);
}




