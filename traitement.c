#include <stdio.h>   // pôur printf
#include "stm32f103xb.h"
#include <stdlib.h>
#include <string.h>

void systemClockConfig(void);
void TIMERS_Config(void);
void tim2_ON_court(void);
void tim2_OFF_court(void);
void tim3_ON_moyen(void);
void tim3_OFF_moyen(void);
void tim4_ON_long(void);
void tim4_OFF_long(void);

int main(void)
{

    systemClockConfig();
	TIMERS_Config();
    
    /* RCC */
	RCC->APB2ENR |= (1 << 2); // Enable RCC for GPIOA Pin/Port
	
	/* MODER */
	GPIOA->CRL &= ~((1 << 22) | (1 << 23) | (1 << 21) | (1 << 20)); // Clear GPIO PA5 MODE

	/* OUTPUT */
	GPIOA->CRL |= (1 << 21); // Set GPIO PA5 MODE (01 : output)
    

	char* morsee[] = { "---", "-...", "-.-.", "-..", ".","..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
	
	char msg[20]={"a  a"};

	//printf("rentrez une phrase: ");
	//fgets(msg, 20, stdin);
	//printf("%d",strlen(msg));
	//msg = argv[1];

	for (int i = 0; i < strlen(msg); i++)
	{
		if(msg[i]==' ')
		{
			//printf("%c",msg[i]);
			tim4_OFF_long();
		}
		else
		{
			char* trad = morsee[msg[i] - 'a'];
            //printf("%s",trad);
		    for (int j = 0; j < strlen(trad); j++)
		    {
				if (trad[j] == '.')
				{
					tim2_ON_court();
					tim2_OFF_court();
				}
				if (trad[j] == '-')
				{
							
					tim3_ON_moyen();
					tim2_OFF_court();
					
					
				}
			}
			tim3_OFF_moyen();
		}
		
	}	
}

void systemClockConfig(void){

    //activation HSI -> HSION
    RCC->CR &= 0;
    RCC->CR |= 1;
    /*Attender la mise en place de HSI HSIRDY*/
    while (!(RCC->CR & (1<<1))){}
    
    // PLL prendre un facteur 9
    RCC->CFGR |= (0b0111 << 18);
    //Activer le PLL -> PLLON
    RCC->CR |= (1 << 24);
    /*Attendre l'activation de PLL-> PLLRDY*/
    while (!(RCC->CR & (1<<25))){}

    /*Metre PLL comme clock source*/ 
    RCC->CFGR &= ~0b11;
    RCC->CFGR |= 0b10;

    /*Attendre le changement de clock source */
    while (!(RCC->CFGR & (0b10<<2))){}

    /*Prescaler 2 pour APB1*/
    RCC->CFGR &= ~(0b111 << 8);
    RCC->CFGR |= 0b100 << 8;

}

void TIMERS_Config(void){
    /*Activer le clock pour TIM2,3,4*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;



    /*Choisir le prescaler pour TIM2,3,4*/
    TIM2->PSC = 35999; /*diviser la frequence source par 36000*/
    TIM3->PSC = 35999;
    TIM4->PSC = 35999;

    /*Choisir la valeur de compteur pour TIM2,3,4*/
    TIM2->ARR = 499; /*interruption tous les 500ms*/
    TIM3->ARR = 999; /*interruption tous les 1000ms*/
    TIM4->ARR = 1999; /*interruption tous les 2000ms*/

    TIM2->CR1 |= TIM_CR1_ARPE;
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM4->CR1 |= TIM_CR1_ARPE;
    TIM2->SR &= ~TIM_SR_UIF;
    TIM3->SR &= ~TIM_SR_UIF;
    TIM4->SR &= ~TIM_SR_UIF;
    
    /*mise a jour des valeurs psc, arr */
    //TIM2->EGR |= 1;
}

void tim2_ON_court(void)
{
	TIM2->CR1 |= TIM_CR1_CEN; //activer timer2
    GPIOA->ODR |= (1 << 5);   //allumer LED
    while(!(TIM2->SR & TIM_SR_UIF)){  //tant y'a pas de débordement reste allumer
        __ASM("nop");
    }
    TIM2->SR &= ~TIM_SR_UIF;   //mettre de le flag à 0
    GPIOA->ODR &= ~(1 << 5);   //etteind LED
    TIM2->CR1 &= ~TIM_CR1_CEN; //désactiver le timer2
}
void tim2_OFF_court(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;  //activer le timer2
    GPIOA->ODR &= ~(1 << 5);   //etteind led
    while(!(TIM2->SR & TIM_SR_UIF)){ //tant y'a pas de débordement reste etteind
        __ASM("nop");
    }
    TIM2->SR &= ~TIM_SR_UIF;  //mettre de le flag à 0
    TIM2->CR1 &= ~TIM_CR1_CEN;  //désactiver le timer2
}
void tim3_ON_moyen(void)
{
	TIM3->CR1 |= TIM_CR1_CEN;
    GPIOA->ODR |= (1 << 5);
    while(!(TIM3->SR & TIM_SR_UIF)){
        __ASM("nop");
    }
    TIM3->SR &= ~TIM_SR_UIF;
     GPIOA->ODR &= ~(1 << 5);
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
void tim3_OFF_moyen(void)
{
	TIM3->CR1 |= TIM_CR1_CEN;
     GPIOA->ODR &= ~(1 << 5);
    while(!(TIM3->SR & TIM_SR_UIF)){
        __ASM("nop");
    }
    TIM3->SR &= ~TIM_SR_UIF;
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
void tim4_ON_long(void)
{
	TIM4->CR1 |= TIM_CR1_CEN;
    GPIOA->ODR |= (1 << 5);
    while(!(TIM4->SR & TIM_SR_UIF)){
        __ASM("nop");
        }
    TIM4->SR &= ~TIM_SR_UIF;
     GPIOA->ODR &= ~(1 << 5);
    TIM4->CR1 &= ~TIM_CR1_CEN;
}
void tim4_OFF_long(void)
{
	TIM4->CR1 |= TIM_CR1_CEN;
     GPIOA->ODR &= ~(1 << 5);
    while(!(TIM4->SR & TIM_SR_UIF)){
        __ASM("nop");
    }
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->CR1 &= ~TIM_CR1_CEN;
}
