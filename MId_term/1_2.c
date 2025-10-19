//dip switch 1번을 on하면 led 8번이 LSB가 되어 오른쪽부터 1씩 up counting된다. 255까지 카운트된 후 다시 0으로 값이 설정된다.
#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int);

// 8비트 비트 반전 함수
unsigned char reverse8(unsigned char b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

int main(void){
    uint32_t g_ui32SysClock;
    unsigned char count = 0;   // LED 카운트 값 (0~255)
    unsigned char switch1 = 0; // 1번 스위치 상태 저장 변수

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN   |
                                         SYSCTL_USE_PLL    |
                                         SYSCTL_CFG_VCO_480),
                                         120000000);

    LED_init();
    DIP_init();
    LED_clear();

    while(1){
        // 1번 스위치 입력 읽기 (예: PA3이 1번 스위치라고 가정)
        switch1 = (GPIO_READ(GPIO_PORTA, 0x08) >> 3);

        if(switch1 == 1){
            // 스위치 ON → 업카운트
            count++;

            // LED 출력 (반전된 비트: LSB가 LED8, MSB가 LED1)
            unsigned char rev_count = reverse8(count);
            GPIO_WRITE(GPIO_PORTL, 0xF,  (rev_count & 0x0F));
            GPIO_WRITE(GPIO_PORTM, 0xF, ((rev_count >> 4) & 0x0F));

            delay(2500000);

            if(count >= 255) {
                count = 0; // 255까지 카운트 후 다시 0으로
            }
        }
        else {
            // 스위치 OFF → 현재 값 유지 (아무것도 안 함)
            // 단, LED는 유지됨
        }
    }
}

void LED_clear(){
    GPIO_WRITE(GPIO_PORTL, 0xf, 0x0);
    GPIO_WRITE(GPIO_PORTM, 0xf, 0x0);
    delay(2500000);
}

void delay(int count){
    while(count != 0){
        count--;
    }
}
