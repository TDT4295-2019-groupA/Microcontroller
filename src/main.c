/***************************************************************************//**
 * @file
 * @brief USB HID keyboard device example.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include <stdbool.h>
#include "../headers/gpio.h"
#include "../headers/timer.h"
#include "../headers/interrupts.h"
#include "em_timer.h"
#include "em_emu.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "segmentlcd.h"
#include "bsp_trace.h"

#include "em_usb.h"


void setupCMU(void);

int main(void)
{

}

void setupCMU(void)
{
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);
	CMU_ClockEnable(cmuClock_DMA, true);
	CMU_ClockEnable(cmuClock_USART1, true);
}

