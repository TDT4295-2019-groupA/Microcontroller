#ifndef HEADERS_GPIO_H_
#define HEADERS_GPIO_H_

#include <stdint.h>
#include <stdbool.h>

#include "em_cmu.h"
#include "em_gpio.h"

#include "defines.h"

#define GPIO_BTN_COUNT 16

void setupGPIO(void);
void handleButtons(void);

bool isButtonDown(unsigned int index);

#endif /* HEADERS_GPIO_H_ */
