//dip switch 1번을 on 하면 led 8개가 binary 값으로 업 카운팅 되면서 led가 점등된다. 0부터 255까지 반복하며 업 카운팅 중 1번 스위치를 off하면 현재 값에서 count를 멈춘다.
//dip switch 8번을 on하면 led 8개가 binary 값으로 다운 카운팅 되면서 led가 점등된다. 255부터 0까지 반복되며 다운 카운팅 중 8번 스위치를 off하면 현재 값에서 count를 멈춘다.
#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int);

int main(void){
    uint32_t g_ui32SysClock;
    unsigned char count = 0;   // LED 카운트 값 (0~255)
    unsigned char switch1 = 0; // 1번 스위치 상태 저장 변수
    unsigned char dip_data = 0;
    unsigned char switch8 = 0;
    unsigned char count2 = 255;

    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                         SYSCTL_OSC_MAIN   |
                                         SYSCTL_USE_PLL    |
                                         SYSCTL_CFG_VCO_480),
                                         120000000);

    LED_init();
    DIP_init();
    LED_clear();

    while(1){
        // DIP 스위치 읽기 (이전과 동일 매핑)
        dip_data = ( GPIO_READ(GPIO_PORTA, 0x08) >> 3 )    // PA3 → bit0
                 | ( GPIO_READ(GPIO_PORTA, 0x40) >> 5 )    // PA6 → bit1
                 | ( GPIO_READ(GPIO_PORTA, 0x80) >> 5 )    // PA7 → bit2
                 | ( GPIO_READ(GPIO_PORTB, 0x08) << 1 )    // PB3 → bit3
                 | ( GPIO_READ(GPIO_PORTQ, 0x10) >> 0 )    // PQ4 → bit4
                 | ( GPIO_READ(GPIO_PORTQ, 0x20) >> 0 )    // PQ5 → bit5
                 | ( GPIO_READ(GPIO_PORTQ, 0x40) >> 0 )    // PQ6 → bit6
                 | ( GPIO_READ(GPIO_PORTG, 0x40) << 1 );   // PG6 → bit7


        // 1번 스위치 입력 읽기 (예: PA3이 1번 스위치라고 가정)
        switch1 = dip_data & 0x01;
        switch8 = (dip_data >> 7) & 0x01;

        if(switch1 == 1){
            // 스위치 ON → 업카운트
            count++;

            // LED 출력 (PL=하위 4bit, PM=상위 4bit)
            GPIO_WRITE(GPIO_PORTL, 0xF,  (count & 0x0F));
            GPIO_WRITE(GPIO_PORTM, 0xF, ((count >> 4) & 0x0F));

            delay(2500000);

            if(count >= 255) {
                count = 0; // 255까지 카운트 후 다시 0으로
            }
        }
        else if(switch8 == 1){
            count2--;

            GPIO_WRITE(GPIO_PORTL, 0xF,  (count2 & 0x0F));
            GPIO_WRITE(GPIO_PORTM, 0xF, ((count2 >> 4) & 0x0F));

            delay(2500000);

            if(count2 <= 0) {
                count2 = 255;
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
