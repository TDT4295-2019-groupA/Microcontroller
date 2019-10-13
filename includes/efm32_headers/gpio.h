#ifndef HEADERS_GPIO_H_
#define HEADERS_GPIO_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_cmu.h"
#include "em_gpio.h"

void setupGPIO(void);
void handleButtons(void);

#endif /* HEADERS_GPIO_H_ */
