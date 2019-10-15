#include "spi.h"
#include "segmentlcd.h"

SPIDRV_HandleData_t handleData;
SPIDRV_Handle_t handle = &handleData;

#define SPIDRV_MASTER_USART3                                           \
  {                                                                    \
    USART1,                     /* USART port                       */ \
    _USART_ROUTE_LOCATION_LOC1, /* USART pins location number       */ \
    4800,                    /* Bitrate                          */ \
    8,                          /* Frame length                     */ \
    0,                          /* Dummy Tx value for Rx only funcs */ \
    spidrvMaster,               /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,     /* Bit order on bus                 */ \
    spidrvClockMode0,           /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,        /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate   /* Slave start transfers immediately*/ \
  }

void TransferComplete( SPIDRV_Handle_t handle,
                       Ecode_t transferStatus,
                       int itemsTransferred )
{
  if ( transferStatus == ECODE_EMDRV_SPIDRV_OK )
  {
	  SegmentLCD_Write("SPI :)"); // yay success
  }
}

void spi_init(void) {
	SPIDRV_Init_t initData = SPIDRV_MASTER_USART3;

	// Initialize a SPI driver instance
	SPIDRV_Init( handle, &initData );
}

void spi_transfer(uint8_t* buffer, uint16_t buffer_size)
{
  // Transmit data using a callback to catch transfer completion.
  SPIDRV_MTransmit( handle, buffer, buffer_size, TransferComplete );
}
