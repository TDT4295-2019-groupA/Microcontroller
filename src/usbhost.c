#include "../headers/usbhost.h"


int connect(void)
{
	STATIC_UBUF(tmpBuf, 1024);
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

void messageloop(void)
{
	int connectionresult;
	unsigned char readbuffer[16] = {0};
	while (USBH_DeviceConnected()) {
		connectionresult = USBH_ReadB(device.ep, readbuffer, 4, 0);
	}
	SegmentLCD_NumberOff();
	SegmentLCD_Write("Device");
	USBTIMER_DelayMs(500);
	SegmentLCD_Write("Removed");
}
