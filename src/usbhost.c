#include "../headers/usbhost.h"


USBH_Device_TypeDef connect(void)
{
	STATIC_UBUF(tmpBuf, 1024);
	USBH_Device_TypeDef device;
	USBH_Ep_TypeDef ep[1];
	USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
	int connectionResult = 1;
	USBH_Init(&is);
	SegmentLCD_Write("hei");
	while (connectionResult != USB_STATUS_OK) {
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
	SegmentLCD_Write("meg");
	return device;
}

void messageloop(USBH_Device_TypeDef device)
{
	int connectionresult;
	unsigned char readbuffer[16] = {0};
	while (USBH_DeviceConnected()) {
		connectionresult = USBH_ReadB(device.ep, readbuffer, 4, 0);
		if (readbuffer[0] != 0) {
			SegmentLCD_Write("ayy");
		}
	}
	SegmentLCD_NumberOff();
	SegmentLCD_Write("Device");
	USBTIMER_DelayMs(500);
	SegmentLCD_Write("Removed");
}
