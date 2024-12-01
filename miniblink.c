#include <miniblink.h>
#include <stm32f10x.h>
#include <common.h>

int8_t LED = 0;
volatile uint8_t FLAG = 1;

extern void * __isr_vector[];

int main(void)
{
    set_system_clock_to_72Mhz();
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CR1 |= TIM_CR1_URS; 
	TIM2->PSC = 36000 - 1;// 1KHz 1ms per 
	
	GPIOA->CRL &= ~((GPIO_CRL_MODE7 | GPIO_CRL_CNF7) | (GPIO_CRL_MODE5 | GPIO_CRL_CNF5));
	GPIOA->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1); 
	GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1);

	GPIOB->CRH &= ~((GPIO_CRH_MODE14 | GPIO_CRH_CNF14));
	GPIOB->CRH |= GPIO_CRH_MODE14_1;
	
	GPIOA->CRL &= ~((GPIO_CRL_MODE1 | GPIO_CRL_CNF1));
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	
	GPIOA->CRH &= ~GPIO_CRH_MODE11;
	GPIOA->CRH |= GPIO_CRH_CNF11_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF11_0;
	GPIOA->ODR &= ~(1 << 11);
	
	AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI11;
	AFIO->EXTICR[2] |= (AFIO_EXTICR3_EXTI11_PA << 12);

	EXTI->RTSR |= EXTI_RTSR_TR11;
	EXTI->FTSR &= ~EXTI_FTSR_TR11;

	EXTI->IMR |= EXTI_IMR_MR11;
	EXTI->IMR |= EXTI_IMR_MR11;

	NVIC->ISER[1] |= (1 << (EXTI15_10_IRQn - 32));  												
    
	spi_init();
	SPI1->CR1 |= SPI_CR1_SPE;

	while(1)
	{
		if((FLAG == 1 && LED == 8) || (FLAG == 0 && LED == -1)) {
			send(0x00);
			GPIOB->ODR |= (1 << 14);
			toggle_led();
			delay_ms(990);
			GPIOB->ODR &= ~(1 << 14);
			delay_ms(10);
			toggle_led();
			LED = FLAG == 1 ? 0 : 7;
		}
		else {
			send((1 << LED));
			LED = (FLAG == 1) ? LED + 1 : LED - 1;
			GPIOB->BSRR = (1 << 14);
			delay_ms(1000);
			GPIOB->BRR = (1 << 14);
		}
	}
    return 0;
}

void spi_init(void) {
	SPI1->CR1 &= SPI_CR1_SPE | SPI_CR1_CRCEN | SPI_CR1_CRCNEXT;
	SPI1->CR1 |= SPI_CR1_MSTR;
	SPI1->CR1 &= ~(SPI_CR1_BR);
	SPI1->CR1 |= SPI_CR1_BR_2;
	
    SPI1->CR1 &= ~SPI_CR1_CPHA;
    SPI1->CR1 &= ~SPI_CR1_CPOL;
	SPI1->CR1 &= ~SPI_CR1_DFF;
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
	SPI1->CR2 |= SPI_CR2_SSOE;
}

void toggle_led() {
    GPIOA->ODR ^= (1 << 1);
}

void send(uint16_t data)
{
    while (!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = data;
	delay_ms(10); 
}

void exti15_10_isr(void) {
    if (EXTI->PR & EXTI_PR_PR11) {
        EXTI->PR = EXTI_PR_PR11; 
        for (volatile int i = 0; i < 10000; i++); 
        FLAG ^= 0x1;
    }
}

void EXTI15_10_Handler(void) {
	exti15_10_isr();
}

void delay_ms(volatile uint16_t delay_ms)
{
	TIM2->ARR = delay_ms - 1;
    TIM2->CR1 |= TIM_CR1_CEN;
	while (!(TIM2->SR & TIM_SR_UIF));
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
}


void set_system_clock_to_72Mhz(void)
{
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    RCC->CFGR |= RCC_CFGR_PLLSRC;
	RCC->CFGR |= RCC_CFGR_PLLMULL9;
	RCC->CFGR &= ~(0b111 << 8);
	RCC->CFGR |= (0b001 << 8); 

    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  
}