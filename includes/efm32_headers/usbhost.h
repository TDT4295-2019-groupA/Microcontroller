#ifndef HEADERS_USBHOST_H_
#define HEADERS_USBHOST_H_

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "buffer.h"
#ifndef DEVICE_SADIE
#include "segmentlcd.h"
#endif

#define USB_OUTPUT_SIZE 4
typedef struct USB_output{
	unsigned char data[USB_OUTPUT_SIZE];
} USB_output;

bool USBConnect(void);
bool USBIsConnected();
cbuf *USBWaitForData();

#endif /* HEADERS_USBHOST_H_ */
