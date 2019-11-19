/*
 * defines.h
 *
 *  Created on: 14. okt. 2019
 *      Author: Eilif
 */

#ifndef INCLUDES_EFM32_HEADERS_DEFINES_H_
#define INCLUDES_EFM32_HEADERS_DEFINES_H_

#define DEVICE_GECKO_STARTER_KIT
//#define DEVICE_SADIE
#define OUTPUT_CLOCK 0  // Enabling this disables UART0 and clocks down the MCU to 16MHz!
#define SPI_GPIO // Defining this outputs SPI on GPIO pins instead of directly to the FPGA.
//#define SPI_FPGA
#define SPI_BITRATE 100000
#define SPI_SPAM 0

#endif /* INCLUDES_EFM32_HEADERS_DEFINES_H_ */
