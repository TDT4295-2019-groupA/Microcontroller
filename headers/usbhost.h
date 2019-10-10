#ifndef HEADERS_USBHOST_H_
#define HEADERS_USBHOST_H_

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_usb.h"
#include "segmentlcd.h"

extern USBH_Device_TypeDef device;
extern USBH_Ep_TypeDef ep[1];

int connect(void);

#endif /* HEADERS_USBHOST_H_ */
