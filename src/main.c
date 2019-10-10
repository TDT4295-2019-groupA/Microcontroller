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
/*
#include <stdbool.h>
#include "../headers/gpio.h"
#include "../headers/timer.h"
#include "../headers/interrupts.h"
#include "../headers/usbhost.h"
#include "em_timer.h"
#include "em_emu.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "segmentlcd.h"
#include "bsp_trace.h"

#include "em_usb.h"
*/
#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "segmentlcd.h"

void setupCMU(void);

STATIC_UBUF(tmpBuf, 1024);
static USBH_Device_TypeDef device;
int main(void)
{
	/*
	STATIC_UBUF(tmpBuf, 1024);
	USBH_Device_TypeDef device;
	*/
	unsigned char lcdbuffer[4] = {0};
	  USB_EndpointDescriptor_TypeDef *retval;
	  USB_EndpointDescriptor_TypeDef checker;
	  USBH_Ep_TypeDef ep[1];
	  int connectionResult;
	  int testytestyoki;
	  unsigned char nullertull = 0;
	  USBH_Init_TypeDef is = USBH_INIT_DEFAULT;

	  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	  CMU_ClockEnable(cmuClock_GPIO, true);

	  SegmentLCD_Init(false);
	  SegmentLCD_Write("USBHOST");
	  SegmentLCD_Symbol(LCD_SYMBOL_GECKO, true);


	  USBH_Init(&is);               /* Initialize USB HOST stack */

	  for (;; ) {
	    /* Wait for device connection */

	    /* Wait for maximum 10 seconds. */
	    connectionResult = USBH_WaitForDeviceConnectionB(tmpBuf, 10);

	    if ( connectionResult == USB_STATUS_OK ) {
	      SegmentLCD_Write("Device");
	      USBTIMER_DelayMs(500);
	      SegmentLCD_Write("Added");
	      USBTIMER_DelayMs(500);

	      if (USBH_QueryDeviceB(tmpBuf, sizeof(tmpBuf), USBH_GetPortSpeed())
	          == USB_STATUS_OK) {
	    	USBH_InitDeviceData(&device, tmpBuf, ep, 1, USBH_GetPortSpeed());
	    	int testy = USBH_AssignHostChannel(ep, 2);
	    	retval = USBH_QGetEndpointDescriptor(tmpBuf, 0, 1, 0);
	    	checker = *retval;
	      } else {
	      }
	      char converted[17];
	      int i;
	      while ( USBH_DeviceConnected() ) {
	    	  testytestyoki = USBH_ReadB(device.ep, lcdbuffer, 4, 0);
	    	  SegmentLCD_Write("yay");
	    	  for(i=0;i<8;i++) {
	    		  sprintf(&converted[i*2], "%02X", lcdbuffer[i]);
	    	  }
	    	  if (lcdbuffer[0] == nullertull) {
	    		  SegmentLCD_Write("123");
	    		  USBTIMER_DelayMs(2000);
	    	  }
	    	  else {
	    		  SegmentLCD_Write(converted);
	    	  }
	      }
	      SegmentLCD_NumberOff();
	      SegmentLCD_Write("Device");
	      USBTIMER_DelayMs(500);
	      SegmentLCD_Write("Removed");
	      USBTIMER_DelayMs(500);
	      SegmentLCD_Write("USBHOST");
	    } else if ( connectionResult == USB_STATUS_DEVICE_MALFUNCTION ) {
	    } else if ( connectionResult == USB_STATUS_TIMEOUT ) {
	      SegmentLCD_Write("TIMEOUT");
	    }
	  }
}

void setupCMU(void)
{
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);
	CMU_ClockEnable(cmuClock_DMA, true);
	CMU_ClockEnable(cmuClock_USART1, true);
}

