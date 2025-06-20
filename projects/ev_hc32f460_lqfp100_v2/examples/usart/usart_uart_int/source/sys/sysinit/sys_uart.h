#ifndef SYS_UART_H_
#define SYS_UART_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 32

#define USART_UNIT                      (CM_USART1)
	
extern uint8_t revbuff[RX_BUFFER_SIZE];
extern uint8_t txbuff[TX_BUFFER_SIZE];

extern volatile  uint32_t comm_len;
extern volatile uint16_t tx_idx;             
extern volatile uint16_t tx_len;             
extern volatile uint8_t isSending;  


int32_t sys_uart_init(void);
void uart_send(uint8_t *data,uint8_t len);

#ifdef __cplusplus
}
#endif

#endif
