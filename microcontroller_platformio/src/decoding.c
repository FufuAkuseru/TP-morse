#include <string.h>
#include "str_to_blink.h"
#include "CLK_Timers.h"
#include "decoding.h"

void decoding(char *receivd_data){
    int loop_octet=receivd_data[0], nb_iter_octet=receivd_data[1], nb_char_octet=receivd_data[5];
    int TIM2_octet=receivd_data[2],TIM3_octet=receivd_data[3],TIM4_octet=receivd_data[4];

    char str[255];
    int j=0;
    while (j<nb_char_octet || receivd_data[j]=='\n')
    {
        strcat(str, receivd_data[j+6]);
        ++j;
    }
    
    
	TIMERS_Config(TIM2_octet,TIM3_octet,TIM4_octet);
    
    if(loop_octet==0){
        nb_iter_octet=1;
    }
    else if(loop_octet==1)
    {
        nb_iter_octet=receivd_data[1];
    }


    for(int i=0; i<nb_iter_octet; ++i){
        traitement(str, nb_char_octet);
    }
}