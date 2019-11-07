#include "defines.h"
#include "usbhost.h"

STATIC_UBUF(tmpBuf, 1024);
static USBH_Device_TypeDef device;
static USBH_Ep_TypeDef ep[1];
static USB_EndpointDescriptor_TypeDef *retval;
static USB_EndpointDescriptor_TypeDef checker;

static unsigned char readbuffer[16 * USB_OUTPUT_SIZE] = {0};

bool USBConnect()
{
	USBH_Init_TypeDef is = USBH_INIT_DEFAULT;
	int connectionResult = 1;
	USBH_Init(&is);
#ifndef DEVICE_SADIE
	SegmentLCD_Write("USB IN");
#endif
	while (connectionResult != USB_STATUS_OK) {
		connectionResult = USBH_WaitForDeviceConnectionB(tmpBuf, 10);
		if ( connectionResult == USB_STATUS_OK ) {
#ifndef DEVICE_SADIE
	    	SegmentLCD_Write("Device");
	    	USBTIMER_DelayMs(500);
	    	SegmentLCD_Write("Added");
	    	USBTIMER_DelayMs(500);
#endif
	    	if (USBH_QueryDeviceB(tmpBuf, sizeof(tmpBuf), USBH_GetPortSpeed())
	          == USB_STATUS_OK) {
	    		USBH_InitDeviceData(&device, tmpBuf, ep, 1, USBH_GetPortSpeed());
	    		int testy = USBH_AssignHostChannel(ep, 2);
	    		retval = USBH_QGetEndpointDescriptor(tmpBuf, 0, 1, 0); 	// This can be removed?
	    		checker = *retval;										// And this?
	    	} else {
	    	}
		}
	}
#ifndef DEVICE_SADIE
	SegmentLCD_Write("meg");
#endif
	return true;
}

bool USBIsConnected(){
	return USBH_DeviceConnected();
}

unsigned char *USBWaitForData(){
	memset(readbuffer, 0, sizeof(readbuffer));
	bool ting = true;
	unsigned char* ptr = readbuffer;
	USBH_ReadB(device.ep, ptr, USB_OUTPUT_SIZE, 0);
	int count = 1;
	while(ting && (ptr < 16)) {
		ptr += USB_OUTPUT_SIZE*sizeof(unsigned char);
		count += 1;
		if(USBH_ReadB(device.ep, ptr, USB_OUTPUT_SIZE, 10) < 0) {
			ting = false;
		}
	}
	return readbuffer;
}
