#ifndef HEADERS_GPIO_H_
#define HEADERS_GPIO_H_

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "em_cmu.h"
#include "em_gpio.h"

#include "defines.h"

#ifdef DEVICE_GECKO_STARTER_KIT
#define GPIO_BTN_COUNT 2
#endif
#ifdef DEVICE_SADIE
#define GPIO_BTN_COUNT 16
#endif

void setupGPIO(void);
void handleButtons(void);

bool isButtonDown(unsigned int index);
void setSoftMute(bool high);
void setExtLed(bool high);

#endif /* HEADERS_GPIO_H_ */
