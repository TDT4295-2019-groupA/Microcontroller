#ifndef HEADERS_USBHOST_H_
#define HEADERS_USBHOST_H_

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "segmentlcd.h"


USBH_Device_TypeDef connect(void);
void messageloop(USBH_Device_TypeDef);

#endif /* HEADERS_USBHOST_H_ */
