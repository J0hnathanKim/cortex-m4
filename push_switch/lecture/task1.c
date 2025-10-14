#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);

int main(void) {
	int push1_current, push2_current, push3_current;
	int push1_prev = 0, push2_prev = 0, push3_prev = 0;

	uint32_t ui32SysClock;
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
											   SYSCTL_CFG_VCO_480), 120000000);

	PUSH_init();
	LED_init();

	LED_clear();

	uint8_t led_count = 0;  // 8비트 카운터

	while(1){
		// Read push button states (active low, so 0 = pressed)
		push1_current = GPIO_READ(GPIO_PORTP, PIN1);  // PUSH_SW 1
		push2_current = GPIO_READ(GPIO_PORTN, PIN3);  // PUSH_SW 2
		push3_current = GPIO_READ(GPIO_PORTE, PIN5);  // PUSH_SW 3

		// PUSH_SW 1: Turn ON LED 1-4 (PORT L)
		if(push1_prev != 0 && push1_current == 0) {
			GPIO_WRITE(GPIO_PORTL, 0xF, 0xF);  // Turn ON all 4 LEDs on PORT L
		}

		// PUSH_SW 2: Binary up counting on LED 1-8
		if(push2_prev != 0 && push2_current == 0) {
			led_count = led_count + 1;  // 8비트 up count (0~255)
			GPIO_WRITE(GPIO_PORTL, 0xF, led_count & 0xF);
			GPIO_WRITE(GPIO_PORTM, 0xF, (led_count >> 4) & 0xF);
		}

		// PUSH_SW 3: Turn OFF all LEDs
		if(push3_prev != 0 && push3_current == 0) {
			GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);  // Turn OFF PORT L LEDs
			GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);  // Turn OFF PORT M LEDs
			led_count = 0; // Optional: reset counter when turning off LEDs
		}

		// Save current state as previous for next iteration
		push1_prev = push1_current;
		push2_prev = push2_current;
		push3_prev = push3_current;

		delay(50000);  // Small delay for debouncing
	}
	return 0;
}

void LED_clear(){
	GPIO_WRITE(GPIO_PORTL, 0xF, 0x0);
	GPIO_WRITE(GPIO_PORTM, 0xF, 0x0);
	delay(2500000);
}

void delay(int count){
	while(count != 0){
		count--;
	}
}
