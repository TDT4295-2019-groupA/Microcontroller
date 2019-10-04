#include "em_usb.h"


SL_ALIGN(4)
const USB_DeviceDescriptor_TypeDef USBDESC_deviceDesc SL_ATTRIBUTE_ALIGN(4) =
{
	//device release number(?)
	.bcdDevice = 0x0000,
	//usb version 2.0
	.bcdUSB = 0x0200,
	//type of descriptor, rest can be found in em_usb header
	.bDescriptorType = USB_DEVICE_DESCRIPTOR,
	//device class. 0 means interface descriptor (config descriptor) specific.
	.bDeviceClass = 0,
	//protocol and subclass can only be 0 when deviceclass is 0
	.bDeviceProtocol = 0,
	.bDeviceSubClass = 0,
	//size of this descriptor in bytes
	.bLength = 18,
	//max packet size for endpoint(s) zero
	.bMaxPacketSize0 = 64,
	//number of configurations supported
	.bNumConfigurations = 1,
	//hardware ids
	.idProduct = 0x0001,
	.idVendor = 0x10C4,
	//human readable descriptors for manufacturer, product and serial number. 0 if no descriptors are used
	.iManufacturer = 0,
	.iProduct = 0,
	.iSerialNumber = 0
};


//contains configuration, interface and endpoint descriptors
SL_ALIGN(4)
const uint8_t USBDESC_configDesc[] SL_ATTRIBUTE_ALIGN(4) =
{
	//configuration descriptor
	//blength: size of current(configuration) descriptor in bytes
	9,
	//bDescriptorType: type of descriptor
	USB_CONFIG_DESCRIPTOR,
	//wTotalLength: length of whole USBDESC_configDesc
	2,
	//bNumInterfaces: number of interfaces (we only need one)
	1,
	//bConfigurationValue: used to set this configuration to active (doesn't matter since we only have one config)
	1,
	//iConfiguration: string descriptor index, 0 for none
	0,
	//bmAttributes: how is the device powered?
	CONFIG_DESC_BM_RESERVED_D7,
	//max power drain
	CONFIG_DESC_MAXPOWER_mA(100),

	//interface descriptor
	//blength: size of current(interface) descriptor in bytes
	9,
	////bDescriptorType: type of descriptor
	USB_INTERFACE_DESCRIPTOR,
	//bInterfaceNumber: index of current interface descriptor. 0 based and incremented by one for each new one
	0,
	//bAlternateSetting: used to select alternate interface, not important in out case
	0,
	//bNumEndpoints: number of endpoints (except control endpoint)
	1,

};


static const USBD_Init_TypeDef usbInitStruct =
{
  .deviceDescriptor    = &USBDESC_deviceDesc,
  .configDescriptor    = USBDESC_configDesc,
  .stringDescriptors   = USBDESC_strings,
  .numberOfStrings     = sizeof(USBDESC_strings) / sizeof(void*),
  .callbacks           = &callbacks,
  .bufferingMultiplier = USBDESC_bufferingMultiplier,
  .reserved            = 0
};
