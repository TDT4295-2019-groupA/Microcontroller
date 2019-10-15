/*
 * dmadrv_config.h
 *
 *  Created on: Oct 15, 2019
 *      Author: sdhgsdfg
 */

#ifndef INCLUDES_EFM32_HEADERS_DMADRV_CONFIG_H_
#define INCLUDES_EFM32_HEADERS_DMADRV_CONFIG_H_

  // DMADRV DMA interrupt priority configuration option.
  // Set DMA interrupt priority. Range is 0..7, 0 is the highest priority.
#define EMDRV_DMADRV_DMA_IRQ_PRIORITY 4
  // DMADRV channel count configuration option.
  // A number of DMA channels to support. A lower DMA channel count will reduce
  // RAM footprint.
#define EMDRV_DMADRV_DMA_CH_COUNT 4

#endif /* INCLUDES_EFM32_HEADERS_DMADRV_CONFIG_H_ */
