#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

#include "driverlib/sysctl.h"

#define PIN0  (0x01)
#define PIN1  (0x01 << 1)
#define PIN2  (0x01 << 2)
#define PIN3  (0x01 << 3)
#define PIN4  (0x01 << 4)
#define PIN5  (0x01 << 5)
#define PIN6  (0x01 << 6)
#define PIN7  (0x01 << 7)

#define GPIO_PORTA   		(0x40058000)
#define GPIO_PORTB 	    	(0x40059000)
#define GPIO_PORTC 		    (0x4005A000)
#define GPIO_PORTD 		    (0x4005B000)
#define GPIO_PORTE 		    (0x4005C000)
//#define GPIO_PORTF 		    (0x40025000)
#define GPIO_PORTF 		    (0x4005D000)
#define GPIO_PORTG 		    (0x4005E000)
#define GPIO_PORTH 		    (0x4005F000)
#define GPIO_PORTJ 		    (0x40060000)
#define GPIO_PORTK 		    (0x40061000)
#define GPIO_PORTL 		    (0x40062000)
#define GPIO_PORTM 		    (0x40063000)
#define GPIO_PORTN 		    (0x40064000)
#define GPIO_PORTQ 		    (0x40066000)

#define GPIO_PORTA_DEN 		(*(volatile unsigned *)(GPIO_PORTA + 0x51C))
#define GPIO_PORTB_DEN 		(*(volatile unsigned *)(GPIO_PORTB + 0x51C))
#define GPIO_PORTC_DEN 		(*(volatile unsigned *)(GPIO_PORTC + 0x51C))
#define GPIO_PORTD_DEN 		(*(volatile unsigned *)(GPIO_PORTD + 0x51C))
#define GPIO_PORTE_DEN 		(*(volatile unsigned *)(GPIO_PORTE + 0x51C))
//#define GPIO_PORTF_DEN 		(*(volatile unsigned *)(GPIO_PORTF + 0x51C))
#define GPIO_PORTF_DEN 		(*(volatile unsigned *)(GPIO_PORTF + 0x51C))
#define GPIO_PORTG_DEN 		(*(volatile unsigned *)(GPIO_PORTG + 0x51C))
#define GPIO_PORTH_DEN 		(*(volatile unsigned *)(GPIO_PORTH + 0x51C))
#define GPIO_PORTJ_DEN 		(*(volatile unsigned *)(GPIO_PORTJ + 0x51C))
#define GPIO_PORTK_DEN 		(*(volatile unsigned *)(GPIO_PORTK + 0x51C))
#define GPIO_PORTL_DEN 		(*(volatile unsigned *)(GPIO_PORTL + 0x51C))
#define GPIO_PORTM_DEN 		(*(volatile unsigned *)(GPIO_PORTM + 0x51C))
#define GPIO_PORTN_DEN 		(*(volatile unsigned *)(GPIO_PORTN + 0x51C))
#define GPIO_PORTQ_DEN 		(*(volatile unsigned *)(GPIO_PORTQ + 0x51C))

#define GPIO_PORTA_DIR 		(*(volatile unsigned *)(GPIO_PORTA + 0x400))
#define GPIO_PORTB_DIR 		(*(volatile unsigned *)(GPIO_PORTB + 0x400))
#define GPIO_PORTC_DIR 		(*(volatile unsigned *)(GPIO_PORTC + 0x400))
#define GPIO_PORTD_DIR 		(*(volatile unsigned *)(GPIO_PORTD + 0x400))
#define GPIO_PORTE_DIR 		(*(volatile unsigned *)(GPIO_PORTE + 0x400))
//#define GPIO_PORTF_DIR 		(*(volatile unsigned *)(GPIO_PORTF + 0x400))
#define GPIO_PORTF_DIR 		(*(volatile unsigned *)(GPIO_PORTF + 0x400))
#define GPIO_PORTG_DIR      (*(volatile unsigned *)(GPIO_PORTG + 0x400))
#define GPIO_PORTH_DIR 		(*(volatile unsigned *)(GPIO_PORTH + 0x400))
#define GPIO_PORTJ_DIR 		(*(volatile unsigned *)(GPIO_PORTJ + 0x400))
#define GPIO_PORTK_DIR 		(*(volatile unsigned *)(GPIO_PORTK + 0x400))
#define GPIO_PORTL_DIR 		(*(volatile unsigned *)(GPIO_PORTL + 0x400))
#define GPIO_PORTM_DIR 		(*(volatile unsigned *)(GPIO_PORTM + 0x400))
#define GPIO_PORTN_DIR 		(*(volatile unsigned *)(GPIO_PORTN + 0x400))
#define GPIO_PORTQ_DIR 		(*(volatile unsigned *)(GPIO_PORTQ + 0x400))

#define GPIO_PORTA_AFSEL 		(*(volatile unsigned *)(GPIO_PORTA + 0x420))
#define GPIO_PORTB_AFSEL 		(*(volatile unsigned *)(GPIO_PORTB + 0x420))
#define GPIO_PORTC_AFSEL 		(*(volatile unsigned *)(GPIO_PORTC + 0x420))
#define GPIO_PORTD_AFSEL		(*(volatile unsigned *)(GPIO_PORTD + 0x420))
#define GPIO_PORTE_AFSEL 		(*(volatile unsigned *)(GPIO_PORTE + 0x420))
//#define GPIO_PORTF_AFSEL 		(*(volatile unsigned *)(GPIO_PORTF + 0x420))
#define GPIO_PORTF_AFSEL 		(*(volatile unsigned *)(GPIO_PORTF + 0x420))
#define GPIO_PORTG_AFSEL        (*(volatile unsigned *)(GPIO_PORTG + 0x420))
#define GPIO_PORTH_AFSEL 		(*(volatile unsigned *)(GPIO_PORTH + 0x420))
#define GPIO_PORTJ_AFSEL 		(*(volatile unsigned *)(GPIO_PORTJ + 0x420))
#define GPIO_PORTK_AFSEL 		(*(volatile unsigned *)(GPIO_PORTK + 0x420))
#define GPIO_PORTL_AFSEL 		(*(volatile unsigned *)(GPIO_PORTL + 0x420))
#define GPIO_PORTM_AFSEL 		(*(volatile unsigned *)(GPIO_PORTM + 0x420))
#define GPIO_PORTN_AFSEL 		(*(volatile unsigned *)(GPIO_PORTN + 0x420))
#define GPIO_PORTQ_AFSEL 		(*(volatile unsigned *)(GPIO_PORTQ + 0x420))

#define GPIO_PORTA_PCTL 		(*(volatile unsigned *)(GPIO_PORTA + 0x52C))
#define GPIO_PORTB_PCTL 		(*(volatile unsigned *)(GPIO_PORTB + 0x52C))
#define GPIO_PORTC_PCTL 		(*(volatile unsigned *)(GPIO_PORTC + 0x52C))
#define GPIO_PORTD_PCTL	    	(*(volatile unsigned *)(GPIO_PORTD + 0x52C))
#define GPIO_PORTE_PCTL 		(*(volatile unsigned *)(GPIO_PORTE + 0x52C))
//#define GPIO_PORTF_PCTL 		(*(volatile unsigned *)(GPIO_PORTF + 0x52C))
#define GPIO_PORTF_PCTL 		(*(volatile unsigned *)(GPIO_PORTF + 0x52C))
#define GPIO_PORTG_PCTL         (*(volatile unsigned *)(GPIO_PORTG + 0x52C))
#define GPIO_PORTH_PCTL 		(*(volatile unsigned *)(GPIO_PORTH + 0x52C))
#define GPIO_PORTJ_PCTL 		(*(volatile unsigned *)(GPIO_PORTJ + 0x52C))
#define GPIO_PORTK_PCTL 		(*(volatile unsigned *)(GPIO_PORTK + 0x52C))
#define GPIO_PORTL_PCTL 		(*(volatile unsigned *)(GPIO_PORTL + 0x52C))
#define GPIO_PORTM_PCTL 		(*(volatile unsigned *)(GPIO_PORTM + 0x52C))
#define GPIO_PORTN_PCTL 		(*(volatile unsigned *)(GPIO_PORTN + 0x52C))
#define GPIO_PORTQ_PCTL 		(*(volatile unsigned *)(GPIO_PORTQ + 0x52C))

//#define GPIO_PORTF_ADCCTL 		(*(volatile unsigned *)(GPIO_PORTF + 0x530))
#define GPIO_PORTF_ADCCTL 		(*(volatile unsigned *)(GPIO_PORTF + 0x530))
#define GPIO_PORTK_ADCCTL 		(*(volatile unsigned *)(GPIO_PORTK + 0x530))

//#define GPIO_PORTF_DMACTL 		(*(volatile unsigned *)(GPIO_PORTF + 0x534))
#define GPIO_PORTF_DMACTL 		(*(volatile unsigned *)(GPIO_PORTF + 0x534))
#define GPIO_PORTK_DMACTL 		(*(volatile unsigned *)(GPIO_PORTK + 0x534))

//#define GPIO_PORTF_PC 		(*(volatile unsigned *)(GPIO_PORTF + 0xFC4))
#define GPIO_PORTF_PC 		(*(volatile unsigned *)(GPIO_PORTF + 0xFC4))
#define GPIO_PORTK_PC 		(*(volatile unsigned *)(GPIO_PORTK + 0xFC4))
#define GPIO_PORTL_PC 		(*(volatile unsigned *)(GPIO_PORTL + 0xFC4))
#define GPIO_PORTM_PC 		(*(volatile unsigned *)(GPIO_PORTM + 0xFC4))
#define GPIO_PORTN_PC 		(*(volatile unsigned *)(GPIO_PORTN + 0xFC4))
#define GPIO_PORTQ_PC 		(*(volatile unsigned *)(GPIO_PORTQ + 0xFC4))


//#define GPIO_PORTF_DR2R 		(*(volatile unsigned *)(GPIO_PORTF + 0x500))
#define GPIO_PORTF_DR2R 		(*(volatile unsigned *)(GPIO_PORTF + 0x500))
#define GPIO_PORTK_DR2R 		(*(volatile unsigned *)(GPIO_PORTK + 0x500))
#define GPIO_PORTL_DR2R 		(*(volatile unsigned *)(GPIO_PORTL + 0x500))
#define GPIO_PORTM_DR2R 		(*(volatile unsigned *)(GPIO_PORTM + 0x500))
#define GPIO_PORTN_DR2R 		(*(volatile unsigned *)(GPIO_PORTN + 0x500))
#define GPIO_PORTQ_DR2R 		(*(volatile unsigned *)(GPIO_PORTQ + 0x500))

//#define GPIO_PORTF_DR4R 		(*(volatile unsigned *)(GPIO_PORTF + 0x504))
#define GPIO_PORTF_DR4R 		(*(volatile unsigned *)(GPIO_PORTF + 0x504))
#define GPIO_PORTK_DR4R 		(*(volatile unsigned *)(GPIO_PORTK + 0x504))
#define GPIO_PORTL_DR4R 		(*(volatile unsigned *)(GPIO_PORTL + 0x504))
#define GPIO_PORTM_DR4R 		(*(volatile unsigned *)(GPIO_PORTM + 0x504))
#define GPIO_PORTN_DR4R 		(*(volatile unsigned *)(GPIO_PORTN + 0x504))
#define GPIO_PORTQ_DR4R 		(*(volatile unsigned *)(GPIO_PORTQ + 0x504))

//#define GPIO_PORTF_DR8R 		(*(volatile unsigned *)(GPIO_PORTF + 0x508))
#define GPIO_PORTF_DR8R 		(*(volatile unsigned *)(GPIO_PORTF + 0x508))
#define GPIO_PORTK_DR8R 		(*(volatile unsigned *)(GPIO_PORTK + 0x508))
#define GPIO_PORTL_DR8R 		(*(volatile unsigned *)(GPIO_PORTL + 0x508))
#define GPIO_PORTM_DR8R 		(*(volatile unsigned *)(GPIO_PORTM + 0x508))
#define GPIO_PORTN_DR8R 		(*(volatile unsigned *)(GPIO_PORTN + 0x508))
#define GPIO_PORTQ_DR8R 		(*(volatile unsigned *)(GPIO_PORTQ + 0x508))

//#define GPIO_PORTF_SLR 		(*(volatile unsigned *)(GPIO_PORTF + 0x518))
#define GPIO_PORTF_SLR 		(*(volatile unsigned *)(GPIO_PORTF + 0x518))
#define GPIO_PORTK_SLR 		(*(volatile unsigned *)(GPIO_PORTK + 0x518))
#define GPIO_PORTL_SLR 		(*(volatile unsigned *)(GPIO_PORTL + 0x518))
#define GPIO_PORTM_SLR 		(*(volatile unsigned *)(GPIO_PORTM + 0x518))
#define GPIO_PORTN_SLR 		(*(volatile unsigned *)(GPIO_PORTN + 0x518))
#define GPIO_PORTQ_SLR 		(*(volatile unsigned *)(GPIO_PORTQ + 0x518))


//#define GPIO_PORTF_DR12R 		(*(volatile unsigned *)(GPIO_PORTF + 0x53C))
#define GPIO_PORTF_DR12R 		(*(volatile unsigned *)(GPIO_PORTF + 0x53C))
#define GPIO_PORTK_DR12R 		(*(volatile unsigned *)(GPIO_PORTK + 0x53C))
#define GPIO_PORTL_DR12R 		(*(volatile unsigned *)(GPIO_PORTL + 0x53C))
#define GPIO_PORTM_DR12R 		(*(volatile unsigned *)(GPIO_PORTM + 0x53C))
#define GPIO_PORTN_DR12R 		(*(volatile unsigned *)(GPIO_PORTN + 0x53C))
#define GPIO_PORTQ_DR12R 		(*(volatile unsigned *)(GPIO_PORTQ + 0x53C))

#define RCGCGPIO     		(*(volatile unsigned *)(0x400FE608))
#define PCGPIO       		(*(volatile unsigned *)(0x400FE908))

#define GPIO_WRITE(reg_addr, pin, data) (*(volatile unsigned *)(reg_addr + (pin<<2))) = data
