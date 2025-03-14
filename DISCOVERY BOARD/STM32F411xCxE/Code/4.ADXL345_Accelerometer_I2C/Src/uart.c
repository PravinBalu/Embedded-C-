#include <UART.h>

#define GPIOAEN		(1U<<0)
#define UART2EN		(1U<<17)

#define CR1_TE		(1U<<3)
#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)

#define SYS_FREQ	16000000
#define APB1_CLK	SYS_FREQ

#define UART_BAUDRATE	115200

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate );
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);

int __io_putchar(int ch){
	uart2_write(ch);
	return ch;
}
void uart2_tx_init(void){

	// configure UART gpio pins
	//configure UART transmission pin
	//enable clock access to gpio a
	//set pa2 mode to alternate function mode
	//set pa2 alternate function type to UART tx (AF07)

	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER  &=~ (1U<<4);
	GPIOA->MODER  |=  (1U<<5);

	GPIOA->AFR[0] |=  (1U<<8);
	GPIOA->AFR[0] |=  (1U<<9);
	GPIOA->AFR[0] |=  (1U<<10);
	GPIOA->AFR[0] &=~ (1U<<11);

	//configure UART module
	//enable clock access to uart1
	//configure baudrate
	//configure the transfer direction
	//enable UART module
	RCC->APB1ENR |= UART2EN;

	uart_set_baudrate( USART2, APB1_CLK, UART_BAUDRATE);
	USART2 ->CR1 |= CR1_TE;
	USART2 ->CR1 |= CR1_UE;
}

void uart2_write(int ch)
{
	while(!(USART2-> SR & SR_TXE)){}
	USART2 -> DR =( ch & 0xFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx-> BRR = compute_uart_bd( PeriphClk, BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
