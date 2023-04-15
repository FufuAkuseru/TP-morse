#include "str_to_blink.h"
#include "CLK_Timers.h"

int main(void){
    int loop_octet=1, nb_iter_octet, second_octet=2, char_octet=2;
    int TIM2_octet=50,TIM3_octet=100,TIM4_octet=1500;
    
    systemClockConfig();
	TIMERS_Config(TIM2_octet,TIM3_octet,TIM4_octet);
    char str[255]={"aa"};
    if(loop_octet==0){
        nb_iter_octet=1;
    }
    else if(loop_octet==1)
    {
        nb_iter_octet=second_octet;
    }


    for(int i=0; i<nb_iter_octet; ++i){
        traitement(str, char_octet);
    }
}