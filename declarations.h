#include <stdint.h>
#include <stdbool.h>

void setupGPIO();
void setupNVIC();
void setupCMU();
void setupTimer(uint32_t delay);
void handleButtons(uint32_t DIN);
void transferComplete(unsigned int channel, bool primary, void *user);
void setupSpi(void);
void setupDma(void);
void spiDmaTransfer(uint8_t *txBuffer, uint8_t *rxBuffer,  int bytes);
bool spiDmaIsActive(void);
void sleepUntilDmaDone(void);

extern int testy;
