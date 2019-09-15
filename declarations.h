#include <stdint.h>


void setupGPIO();
void setupNVIC();
void setupTimer(uint32_t delay);
void handleButtons(uint32_t DIN);

extern int testy;
