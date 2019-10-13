#ifndef HEADERS_SPI_MASTER_H_
#define HEADERS_SPI_MASTER_H_

#include <stdbool.h>

#include "em_usart.h"
#include "em_gpio.h"
#include "em_dma.h"
#include "em_emu.h"
#include "em_int.h"
#include "dmactrl.h"

void transferComplete(unsigned int channel, bool primary, void *user);
void setupSpi(void);
void setupDma(void);
void spiDmaTransfer(uint8_t *txBuffer, uint8_t *rxBuffer,  int bytes);
bool spiDmaIsActive(void);
void sleepUntilDmaDone(void);

#endif /* HEADERS_SPI_MASTER_H_ */
