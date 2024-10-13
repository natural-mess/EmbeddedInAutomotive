#include <stm32l476xx.h>

void delay(__IO uint32_t timedelay){ 
	for(uint32_t i=0; i < timedelay; i++){}
}


int main()
{
	// 1. cau hinh clock cho GPIO A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// 2. cau hinh output cho PA5 (PA5: general purpose output mode)
	// MODE5[1:0] 
	// set bit 10 thanh 1 va set bit 11 thanh 0, nhu vay MODE5[1:0] se co gia tri 01 tuc la output
	GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE5_0;
	
	while (1)
	{
		// bat den LD2 tai PA5
		GPIOA->ODR |= GPIO_ODR_OD5;
		//delay
		delay(100000);
		// tat den LD2 tai PA5
		GPIOA->ODR &= ~GPIO_ODR_OD5;
		//delay
		delay(100000);
	}
	return 0;
}