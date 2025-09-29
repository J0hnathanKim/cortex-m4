#include "cortex_m4.h"
#include "MyLib.h"

void LED_clear();
void delay(int);

int main(void){
    uint32_t g_ui32SysClock;
    volatile unsigned char count = 255; // 255부터 시작 (8비트: 0~255)
    unsigned char dip_data = 0;
    unsigned char sw8 = 0;              // 8번 스위치 상태

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

        // 8번 스위치 상태 (bit7)
        sw8 = (dip_data >> 7) & 0x01;

        if (sw8) {
            // 스위치 ON일 때만 다운카운트
            // LED 출력 (PL=하위4비트, PM=상위4비트)
            GPIO_WRITE(GPIO_PORTL, 0xF,  (count & 0x0F));
            GPIO_WRITE(GPIO_PORTM, 0xF, ((count >> 4) & 0x0F));

            delay(2500000);

            if (count == 0) {
                count = 255; // 0에서 한 바퀴 돌아 255로
            } else {
                count--;     // 다운카운트
            }
        } else {
            // 스위치 OFF → 현재 값 유지 (LED 그대로 유지)
            // 필요시 아래 두 줄로 '유지 출력'을 명시해도 됨(시각적 변화 없음)
            GPIO_WRITE(GPIO_PORTL, 0xF,  (count & 0x0F));
            GPIO_WRITE(GPIO_PORTM, 0xF, ((count >> 4) & 0x0F));
            delay(2500000);
        }
    }
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
