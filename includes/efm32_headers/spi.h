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

void TransferComplete( SPIDRV_Handle_t handle,
                       Ecode_t transferStatus,
                       int itemsTransferred );

Ecode_t spi_initialize(void);

// sends a test packet
void spi_test(uint8_t data_size);

// if you just want to forget about it, provide TransferComplete as callback.
void spi_transmit_callback(uint8_t* data, SPIDRV_Callback_t transfer_callback);

#endif /* INCLUDES_EFM32_HEADERS_SPI_H_ */
