#include "cortex_m4.h"
#include "myLib.h"

void delay(int count);
uint32_t g_ui32SysClock;
int main(void) {
    int hz = 10000;   // 초기 주파수



    g_ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                       SYSCTL_CFG_VCO_480), 120000000);
    BUZZER_init();

    GPIO_WRITE(GPIO_PORTD, 0x10, 0x10);  // buzzer ON
    int noteDelay[] = {
          9500,  // 도
          8500,  // 레
          7600,  // 미
          7100,  // 파
          6300,  // 솔
          5600,  // 라
          5000,  // 시
          4750   // 높은 도
      };

      int i, j;
      const int numNotes = 8;
      const int noteDuration = 200;   // 각 음을 얼마나 길게 낼지 (토글 반복 횟수)

      while (1) {
          // 도 레 미 파 솔 라 시 도 순서대로 재생
          for (i = 0; i < numNotes; i++) {
              int d = noteDelay[i];

              // 하나의 음을 일정 시간동안 유지
              for (j = 0; j < noteDuration; j++) {
                  GPIO_WRITE(GPIO_PORTB, 0x04, 0x04);   // buzzer ON (출력 HIGH)
                  delay(d);

                  GPIO_WRITE(GPIO_PORTB, 0x04, ~0x04);  // buzzer OFF (출력 LOW)
                  delay(d);
              }

              // 음과 음 사이에 약간의 공백(쉼표 느낌)
              delay(30000);
          }
      }

    GPIO_WRITE(GPIO_PORTD, 0x10, ~0x10);  // buzzer OFF

    return 0;
}


