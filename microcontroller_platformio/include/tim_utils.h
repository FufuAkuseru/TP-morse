#ifndef TIM_UTILS_h
#define TIM_UTILS_h

#include <stdbool.h>

void start_timer_short();
void start_timer_medium();
void start_timer_long();

void stop_timer_short();
void stop_timer_medium();
void stop_timer_long();

bool is_timer_done_short();
bool is_timer_done_medium();
bool is_timer_done_long();

#endif