#define STM32F10X_MD
#define USE_STDPERIPH_DRIVER
#include <stdint.h>

void SystemInit(void);
int main(void);
void delay_ms(volatile uint16_t delay_ms);
void send(uint16_t data);
void exti15_10_isr(void);
void EXTI15_10_Handler(void);
void spi_init(void);
void toggle_led();