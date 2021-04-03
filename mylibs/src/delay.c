#include "main.h"

unsigned long long int tickms = 0;

void delay_ms(int delay){
  unsigned long long int ticksStart = tickms;
  while((tickms - ticksStart) < delay){}
}

unsigned long long int get_tickms(void){
  return tickms;
}

void tick_ms(void){
  tickms++;
}
