#include "str_to_blink.h"  
#include "CLK_Timers.h"


void traitement(uint8_t *str, int char_octet)
{


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

