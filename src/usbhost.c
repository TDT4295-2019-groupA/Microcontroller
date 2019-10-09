

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "segmentlcd.h"


STATIC_UBUF(tmpBuf, 1024);
static USBH_Device_TypeDef device;
static USBH_Ep_TypeDef ep[1];

int connect(void)
{
	USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
	int connectionResult;
	USBH_Init(&is);

	while (connectionResult != 1) {
		connectionResult = USBH_WaitForDeviceConnectionB(tmpBuf, 10);
		if ( connectionResult == USB_STATUS_OK ) {
	    	SegmentLCD_Write("Device");
	    	USBTIMER_DelayMs(500);
	    	SegmentLCD_Write("Added");
	    	USBTIMER_DelayMs(500);

	    	if (USBH_QueryDeviceB(tmpBuf, sizeof(tmpBuf), USBH_GetPortSpeed())
	          == USB_STATUS_OK) {
	    		USBH_InitDeviceData(&device, tmpBuf, ep, 1, USBH_GetPortSpeed());
	    		USBH_AssignHostChannel(ep, 2);
	    	} else {
	    	}
		}
	}
	return connectionResult;
}
