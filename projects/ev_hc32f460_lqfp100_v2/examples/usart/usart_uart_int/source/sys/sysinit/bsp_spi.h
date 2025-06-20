#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

//#define SPI_BUF_LEN             (1024UL)
//static char u8TxBuf[SPI_BUF_LEN];

#define SPI_UNIT                        (CM_SPI3)

/* LCD_CS = PB7*/
#define LCD_CS_PORT                     (GPIO_PORT_B)
#define LCD_CS_PIN                      (GPIO_PIN_07)
//#define LCD_CS_FUNC                     (GPIO_FUNC_42)

#define LCD_CS_0()      								(GPIO_ResetPins(LCD_CS_PORT, LCD_CS_PIN))
#define LCD_CS_1()      								(GPIO_SetPins(LCD_CS_PORT, LCD_CS_PIN))

#define DMA_UNIT              (CM_DMA1)
#define DMA_CLK               (FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS)
#define DMA_TX_CH             (DMA_CH0)
#define DMA_TX_TRIG_CH        (AOS_DMA1_0)


void SPI_Config(void);
// void SPI_SendByte(uint8_t byte);

#endif
