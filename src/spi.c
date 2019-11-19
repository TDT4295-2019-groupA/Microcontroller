#include "spi.h"

SPIDRV_HandleData_t handleData;
SPIDRV_Handle_t handle = &handleData;

#ifdef DEVICE_SADIE
#ifdef SPI_GPIO
#undef SPIDRV_MASTER_USART0

#define SPIDRV_MASTER_USART0                                           \
  {                                                                    \
    USART0,                     /* USART port                       */ \
    _USART_ROUTELOC0_TXLOC_LOC0, /* USART Tx pin location number    */ \
    _USART_ROUTELOC0_RXLOC_LOC0, /* USART Rx pin location number    */ \
    _USART_ROUTELOC0_CLKLOC_LOC0, /* USART Clk pin location number  */ \
    _USART_ROUTELOC0_CSLOC_LOC0, /* USART Cs pin location number    */ \
    SPI_BITRATE,                    /* Bitrate                          */ \
    8,                          /* Frame length                     */ \
    0,                          /* Dummy Tx value for Rx only funcs */ \
    spidrvMaster,               /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,     /* Bit order on bus                 */ \
    spidrvClockMode0,           /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,        /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate   /* Slave start transfers immediately*/ \
  }
#endif
#ifdef SPI_FPGA
#undef SPIDRV_MASTER_USART1
#define SPIDRV_MASTER_USART1                                           \
  {                                                                    \
    USART1,                     /* USART port                       */ \
    _USART_ROUTELOC0_TXLOC_LOC1, /* USART Tx pin location number   */ \
    _USART_ROUTELOC0_RXLOC_LOC1, /* USART Rx pin location number   */ \
    _USART_ROUTELOC0_CLKLOC_LOC1,/* USART Clk pin location number  */ \
    _USART_ROUTELOC0_CSLOC_LOC1, /* USART Cs pin location number   */ \
    SPI_BITRATE,                       /* Bitrate                          */ \
    8,                          /* Frame length                     */ \
    0,                          /* Dummy Tx value for Rx only funcs */ \
    spidrvMaster,               /* SPI mode                         */ \
    spidrvBitOrderMsbFirst,     /* Bit order on bus                 */ \
    spidrvClockMode0,           /* SPI clock/phase mode             */ \
    spidrvCsControlAuto,        /* CS controlled by the driver      */ \
    spidrvSlaveStartImmediate   /* Slave start transfers immediately*/ \
  }
#endif
#endif

void TransferComplete( SPIDRV_Handle_t handle,
                       Ecode_t transferStatus,
                       int itemsTransferred )
{}

void spi_init(void) {
#ifdef SPI_GPIO
	SPIDRV_Init_t initData = SPIDRV_MASTER_USART0;
#endif
#ifdef SPI_FPGA
	SPIDRV_Init_t initData = SPIDRV_MASTER_USART1;
#endif
	// Initialize a SPI driver instance
	SPIDRV_Init( handle, &initData );
}


void spi_transmit(uint8_t* buffer, uint16_t buffer_size)
{
  // Transmit data using a callback to catch transfer completion.
  // to do nonblocking transmit instead, use SPIDRV_MTransmit and add the callback to the function call
	SPIDRV_MTransmitB( handle, buffer, buffer_size);
}
