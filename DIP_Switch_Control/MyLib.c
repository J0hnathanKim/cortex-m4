/*
 * MyLib.c
 *
 *  Created on: 2018. 3. 08.
 *      Author: ESL-KMC
 */


#include "MyLib.h"
#include "cortex_m4.h"

void LED_init(void){
    // 1) GPIO 클럭 인가: Port L(비트10), Port M(비트11)
    RCGCGPIO |= (1U << 10) | (1U << 11);

    // 2) 대체 기능 해제 → 일반 GPIO
    GPIO_PORTL_AFSEL &= ~0x0F;  // PL3..PL0
    GPIO_PORTM_AFSEL &= ~0x0F;  // PM3..PM0

    // 3) 핀 멀티플렉스: GPIO는 PCTL nibble을 0으로
    GPIO_PORTL_PCTL &= ~0x0000FFFF; // PL0~PL3
    GPIO_PORTM_PCTL &= ~0x0000FFFF; // PM0~PM3

    // 4) 방향: 출력
    GPIO_PORTL_DIR |= 0x0F;
    GPIO_PORTM_DIR |= 0x0F;

    // 5) 디지털 기능 활성화
    GPIO_PORTL_DEN |= 0x0F;
    GPIO_PORTM_DEN |= 0x0F;

    // (선택) 구동/슬루 설정: 필요 최소만
    GPIO_PORTL_DR2R &= ~0x0F;  GPIO_PORTM_DR2R &= ~0x0F;
    GPIO_PORTL_DR4R |=  0x0F;  GPIO_PORTM_DR4R |=  0x0F;   // 4mA
    GPIO_PORTL_DR8R &= ~0x0F;  GPIO_PORTM_DR8R &= ~0x0F;   // 8mA 해제
    GPIO_PORTL_SLR  &= ~0x0F;  GPIO_PORTM_SLR  &= ~0x0F;   // Slew 제한 해제
    GPIO_PORTL_DR12R &= ~0x0F; GPIO_PORTM_DR12R &= ~0x0F;  // 12mA 해제
}

void DIP_init(void) {
    // 1) 클럭 인가
    RCGCGPIO |= (1U << 0) | (1U << 1) | (1U << 6) | (1U << 14);

    // 2) 방향: 입력으로 설정
    GPIO_PORTA_DIR &= ~0xFF;  // PA 핀 입력
    GPIO_PORTB_DIR &= ~0xFF;  // PB 핀 입력
    GPIO_PORTG_DIR &= ~0xFF;  // PG 핀 입력
    GPIO_PORTN_DIR &= ~0xFF;  // PN 핀 입력

    // 3) 디지털 기능 활성화
    GPIO_PORTA_DEN |= 0xFF;
    GPIO_PORTB_DEN |= 0xFF;
    GPIO_PORTG_DEN |= 0xFF;
    GPIO_PORTN_DEN |= 0xFF;

    void DIP_init(void) {
    // 1) 클럭 인가
    RCGCGPIO |= (1U << 0) | (1U << 1) | (1U << 6) | (1U << 14);

    // 2) 방향: 입력으로 설정
    GPIO_PORTA_DIR &= ~0xFF;  // PA 핀 입력
    GPIO_PORTB_DIR &= ~0xFF;  // PB 핀 입력
    GPIO_PORTG_DIR &= ~0xFF;  // PG 핀 입력
    GPIO_PORTN_DIR &= ~0xFF;  // PN 핀 입력

    // 3) 디지털 기능 활성화
    GPIO_PORTA_DEN |= 0xFF;
    GPIO_PORTB_DEN |= 0xFF;
    GPIO_PORTG_DEN |= 0xFF;
    GPIO_PORTN_DEN |= 0xFF;

    GPIO_PORTA_AFSEL &= ~0xFF;
    GPIO_PORTB_AFSEL &= ~0xFF;
    GPIO_PORTG_AFSEL &= ~0xFF;
    GPIO_PORTN_AFSEL &= ~0xFF;

}


