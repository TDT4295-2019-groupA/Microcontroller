#include <stdint.h>
#include <stdbool.h>

void setupGPIO(void);
void setupNVIC(void);
void setupCMU(void);
void setupTimer(uint16_t delay);
void handleButtons(uint32_t DIN);
void transferComplete(unsigned int channel, bool primary, void *user);
void setupSpi(void);
void setupDma(void);
void spiDmaTransfer(uint8_t *txBuffer, uint8_t *rxBuffer,  int bytes);
bool spiDmaIsActive(void);
void sleepUntilDmaDone(void);

extern int testy;
extern bool timerStarted;
