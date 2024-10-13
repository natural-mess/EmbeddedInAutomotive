#include <stm32l476xx.h>

#define HIGH 1
#define LOW  0

void delay(__IO uint32_t timedelay){ 
	for(uint32_t i=0; i < timedelay; i++){}
}

void WritePin(GPIO_TypeDef *GPIO_Port, uint8_t Pin, uint8_t state)
{
	if(state == HIGH)
	GPIO_Port->ODR |= (1<<Pin);
	else
	GPIO_Port->ODR &= ~(1<<Pin);
}

void GPIO_Config(void)
{			
    // Cau hinh output cho PA5 (PA5: general purpose output mode)
	// MODE5[1:0] 
	// set bit 10 thanh 1 va set bit 11 thanh 0, nhu vay MODE5[1:0] se co gia tri 01 tuc la output
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	// Cau hinh input cho PC13
	GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;
	
	// cau hinh pull up cho PC13 (01)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_0;
}

void RCC_Config(void)
{			
    // Cau hinh clock cho GPIO A (PA5 la LED)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Cau hinh clock cho GPIO C (PC13 la nut bam)
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
}

void toggle(void)
{
	if (GPIOA->ODR & GPIO_ODR_OD5) 
	{
		GPIOA->ODR &= ~GPIO_ODR_OD5;
	}
	else
	{
		GPIOA->ODR |= GPIO_ODR_OD5;
	}
}

int main()
{
	RCC_Config();
	GPIO_Config();

	while (1)
	{
		if ((GPIOC->IDR & (1<<13)) == 0)
		{
			while ((GPIOC->IDR & (1<<13)) == 0);
			toggle();
		}
	}
	return 0;
}