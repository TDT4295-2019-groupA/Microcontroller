#ifndef HEADERS_TIMER_H_
#define HEADERS_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_cmu.h"
#include "em_timer.h"

void setupTimer(uint16_t delay);
void pulse(void);
bool setDone(void);

#endif /* HEADERS_TIMER_H_ */
