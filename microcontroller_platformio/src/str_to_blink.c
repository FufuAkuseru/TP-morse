#include "str_to_blink.h"  
#include "CLK_Timers.h"


void traitement(char *str, int char_octet)
{

    /* RCC */
	RCC->APB2ENR |= (1 << 2); // Enable RCC for GPIOA Pin/Port
	
	/* MODER */
	GPIOA->CRL &= ~((1 << 22) | (1 << 23) | (1 << 21) | (1 << 20)); // Clear GPIO PA5 MODE

	/* OUTPUT */
	GPIOA->CRL |= (1 << 21); // Set GPIO PA5 MODE (01 : output)
    

	char* morsee[] = { "---", "-...", "-.-.", "-..", ".","..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." };
	
	char* msg;
    msg=str;

	//printf("rentrez une phrase: ");
	//fgets(msg, 20, stdin);
	//printf("%d",strlen(msg));
	//msg = argv[1];

	for (int i = 0; i < char_octet; i++)
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

