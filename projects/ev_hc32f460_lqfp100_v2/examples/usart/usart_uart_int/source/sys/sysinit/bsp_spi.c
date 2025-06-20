#include "main.h"
#include "bsp_spi.h"

/* SPI definition */
#define SPI_UNIT                        (CM_SPI3)
#define SPI_CLK                         (FCG1_PERIPH_SPI3)
#define SPI_TX_IRQ_SRC                  (INT_SRC_SPI3_SPTI)
#define SPI_TX_IRQ_NUM                  (INT006_IRQn)	
#define SPI_TX_EVT_SRC 									(EVT_SRC_SPI3_SPTI)
#define SPI_RX_EVT_SRC                  (EVT_SRC_SPI3_SPRI)

/* SS = PB7 CS*/
#define SPI_SS_PORT                     (GPIO_PORT_B)
#define SPI_SS_PIN                      (GPIO_PIN_07)
#define SPI_SS_FUNC                     (GPIO_FUNC_42)
/* SCK = PB8 SCL*/
#define SPI_SCK_PORT                    (GPIO_PORT_B)
#define SPI_SCK_PIN                     (GPIO_PIN_08)
#define SPI_SCK_FUNC                    (GPIO_FUNC_43)
/* MOSI = PB9 SDA*/
#define SPI_MOSI_PORT                   (GPIO_PORT_B)
#define SPI_MOSI_PIN                    (GPIO_PIN_09)
#define SPI_MOSI_FUNC                   (GPIO_FUNC_40)

/* DMA definition */
#define DMA_UNIT              (CM_DMA1)
#define DMA_CLK               (FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS)
#define DMA_TX_CH             (DMA_CH0)
#define DMA_TX_TRIG_CH        (AOS_DMA1_0)


static __IO uint8_t dma_transfer_complete = 0;
static __IO uint8_t u8Txdata;
static __IO uint8_t u8TxComplete = 1; 

/*
static void SPI_TransCompleteCallback(void)
{
	    u8TxComplete = 1;
      SPI_IntCmd(SPI_UNIT, SPI_INT_TX_BUF_EMPTY, DISABLE);
}
*/

/*
* SPI DMA configure.
*/
void SPI_DMA_Config(void)
{
    stc_dma_init_t stcDmaInit;

    // 使能 DMA 时钟
    FCG_Fcg0PeriphClockCmd(DMA_CLK, ENABLE);
    (void)DMA_StructInit(&stcDmaInit);

    // 配置 TX DMA 通道
    stcDmaInit.u32BlockSize     = 1UL;
    stcDmaInit.u32TransCount    = 0;                  // 暂不设，传输时再设置
    stcDmaInit.u32DataWidth     = DMA_DATAWIDTH_8BIT;
    stcDmaInit.u32SrcAddrInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestAddrInc   = DMA_DEST_ADDR_FIX;
    stcDmaInit.u32SrcAddr       = 0;                  // 传输时再设
    stcDmaInit.u32DestAddr      = (uint32_t)(&SPI_UNIT->DR);
    stcDmaInit.u32IntEn         = DMA_INT_DISABLE;

    if (LL_OK != DMA_Init(DMA_UNIT, DMA_TX_CH, &stcDmaInit)) {
        for (;;) {
        }
    }
    AOS_SetTriggerEventSrc(DMA_TX_TRIG_CH, SPI_TX_EVT_SRC);
    DMA_Cmd(DMA_UNIT, ENABLE);
}

/**
 * @brief  SPI configure.
 * @param  None
 * @retval None
 */
void SPI_Config(void)
{
    stc_spi_init_t stcSpiInit;
    // stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv       = PIN_HIGH_DRV;
		stcGpioInit.u16PinOutputType = PIN_OUT_TYPE_CMOS;
		stcGpioInit.u16PinDir        = PIN_DIR_OUT;
    //(void)GPIO_Init(SPI_SS_PORT,   SPI_SS_PIN,   &stcGpioInit);
    (void)GPIO_Init(SPI_SCK_PORT,  SPI_SCK_PIN,  &stcGpioInit);
    (void)GPIO_Init(SPI_MOSI_PORT, SPI_MOSI_PIN, &stcGpioInit);
    //(void)GPIO_Init(SPI_MISO_PORT, SPI_MISO_PIN, &stcGpioInit);

    /* Configure Port */
   // GPIO_SetFunc(SPI_SS_PORT,   SPI_SS_PIN,   SPI_SS_FUNC);
    GPIO_SetFunc(SPI_SCK_PORT,  SPI_SCK_PIN,  SPI_SCK_FUNC);
    GPIO_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_FUNC);
    //GPIO_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_FUNC);

    /* Configuration SPI */
    FCG_Fcg1PeriphClockCmd(SPI_CLK, ENABLE);
    SPI_StructInit(&stcSpiInit);
    stcSpiInit.u32WireMode          = SPI_3_WIRE;
    stcSpiInit.u32TransMode         = SPI_SEND_ONLY;
    stcSpiInit.u32MasterSlave       = SPI_MASTER;
    stcSpiInit.u32Parity            = SPI_PARITY_INVD;
    stcSpiInit.u32SpiMode           = SPI_MD_3;
    stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV2;
    stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
    stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
    stcSpiInit.u32FrameLevel        = SPI_1_FRAME;
    (void)SPI_Init(SPI_UNIT, &stcSpiInit);
    // SPI_IntCmd(SPI_UNIT, (SPI_INT_TX_BUF_EMPTY | SPI_INT_RX_BUF_FULL), ENABLE);
		SPI_IntCmd(SPI_UNIT, SPI_INT_TX_BUF_EMPTY, ENABLE);

    /* TX NVIC configure 
    stcIrqSignConfig.enIntSrc    = SPI_TX_IRQ_SRC;
    stcIrqSignConfig.enIRQn      = SPI_TX_IRQ_NUM;
    stcIrqSignConfig.pfnCallback = &SPI_TransCompleteCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
		*/
    /* RX NVIC configure 
    stcIrqSignConfig.enIntSrc    = SPI_RX_IRQ_SRC;
    stcIrqSignConfig.enIRQn      = SPI_RX_IRQ_NUM;
    stcIrqSignConfig.pfnCallback = &SPI_ReceiveCompleteCallback;
		
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT - 1);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
		*/
		SPI_Cmd(SPI_UNIT, ENABLE);	
    // SPI_DMA_Config(); 
}	



/*
void SPI_SendByte(uint8_t byte)
{
	
				//u8Txdata = byte;
				//SPI_CS_0();
				//u8TxComplete = 0;
				//SPI_IntCmd(SPI_UNIT, SPI_INT_TX_BUF_EMPTY, ENABLE);
	
	SPI_CS_0();
	SPI_WriteData(SPI_UNIT, byte);
	// while(RESET == SPI_GetStatus(SPI_UNIT, SPI_FLAG_TX_BUF_EMPTY));
	// while (u8TxComplete == 0) {}
	SPI_CS_1();
}

*/


