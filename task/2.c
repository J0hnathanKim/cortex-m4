#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int count);
void LED_display(unsigned char value);

int main(void) {
	int dip1_state;
	unsigned char count = 0;

	uint32_t ui32SysClock;
	// Run from the PLL at 120 MHz.
	ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
											   SYSCTL_CFG_VCO_480), 120000000);

	DIP_init();
	LED_init();

	LED_clear();

	while(1){
		// Read DIP_SW 1 state (PA3)
		// DIP switch is active high (ON = 1, OFF = 0)
		dip1_state = GPIO_READ(GPIO_PORTA, PIN3);

		if(dip1_state != 0) {  // DIP_SW 1 is ON
			// Display current count value on LEDs
			LED_display(count);

			// Increment counter
			count++;

			// Reset to 0 after reaching 255
			if(count > 255) {
				count = 0;
			}

			// Delay for visible counting (adjust for desired speed)
			delay(3000000);  // About 0.2 second delay at 120MHz
		}
		else {  // DIP_SW 1 is OFF - stop counting
			// Keep displaying current value
			LED_display(count);
		}

		delay(10000);  // Small delay for switch reading
	}
	return 0;
}

void LED_display(unsigned char value) {
	// LED 8-5 (LSB side) on PORT M (bits 3-0)
	// LED 4-1 (MSB side) on PORT L (bits 3-0)

	unsigned char lower_nibble = value & 0x0F;        // bits 3-0 for LED 8-5
	unsigned char upper_nibble = (value >> 4) & 0x0F; // bits 7-4 for LED 4-1

	// PORT M: LED 8(bit0), 7(bit1), 6(bit2), 5(bit3)
	GPIO_WRITE(GPIO_PORTM, 0xF, lower_nibble);

	// PORT L: LED 4(bit0), 3(bit1), 2(bit2), 1(bit3)
	GPIO_WRITE(GPIO_PORTL, 0xF, upper_nibble);
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
