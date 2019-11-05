/*
 * spi.h
 *
 *  Created on: Oct 14, 2019
 *      Author: sdhgsdfg
 */

#ifndef INCLUDES_EFM32_HEADERS_SPI_H_
#define INCLUDES_EFM32_HEADERS_SPI_H_

#include <stdio.h>
#include "spidrv.h"
#include "defines.h"

void TransferComplete( SPIDRV_Handle_t handle,
                       Ecode_t transferStatus,
                       int itemsTransferred );

void spi_init(void);

// if you just want to forget about it, provide TransferComplete as callback.
void spi_transmit(uint8_t* data, uint16_t data_size);

#endif /* INCLUDES_EFM32_HEADERS_SPI_H_ */
