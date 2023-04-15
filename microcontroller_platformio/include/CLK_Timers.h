#ifndef CLK_TIMERS_h
#define CLK_TIMERS_h
#include "stm32f103xb.h"

void systemClockConfig();
void TIMERS_Config(int TIM2_val, int TIM3_val, int TIM4_val);
void tim2_ON_court(void);
void tim2_OFF_court(void);
void tim3_ON_moyen(void);
void tim3_OFF_moyen(void);
void tim4_ON_long(void);
void tim4_OFF_long(void);



#endif