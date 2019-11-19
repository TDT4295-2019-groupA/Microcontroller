#include "spi.h"
#include "segmentlcd.h"

SPIDRV_HandleData_t handleData;
SPIDRV_Handle_t handle = &handleData;




void TransferComplete( SPIDRV_Handle_t handle,
                       Ecode_t transferStatus,
                       int itemsTransferred )
{
  if ( transferStatus == ECODE_EMDRV_SPIDRV_OK )
  {
	  //SegmentLCD_Write("SPI"); // yay success
  }
}

void spi_init(void) {
	SPIDRV_Init_t initData = SPIDRV_MASTER_USART1;

	// Initialize a SPI driver instance
	SPIDRV_Init( handle, &initData );
}


void spi_transmit(uint8_t* buffer, uint16_t buffer_size)
{
  // Transmit data using a callback to catch transfer completion.
  // to do blocking transmit instead, use SPIDRV_MTransmitB and remove the callback from the function call
	while(true) {
		SPIDRV_MTransmitB( handle, buffer, buffer_size);
	}
}
