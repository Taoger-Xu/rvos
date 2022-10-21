//
// low-level driver routines for 16550a UART.
// uart串口配置文件
//
#include "platform.h"
#include "types.h"

/*
 * The UART control registers are memory-mapped at address UART0. 
 * This macro returns the address of one of the registers.
 */
#define UART_REG(reg) ((volatile uint8_t *)(UART0 + reg))

/*
 * 参考:http://byterunner.com/16550.html, UART control registers 手册
 * 有的寄存器有两种不同的工作模式，读模式和写模式
 * 0 (write mode): THR/DLL
 * 1 (write mode): IER/DLM
 */

/*
 * POWER UP DEFAULTS
 * IER = 0: TX/RX holding register interrupts are both disabled
 * ISR = 1: no interrupt penting
 * LCR = 0
 * MCR = 0
 * LSR = 60 HEX
 * MSR = BITS 0-3 = 0, BITS 4-7 = inputs
 * FCR = 0
 * TX = High
 * OP1 = High
 * OP2 = High
 * RTS = High
 * DTR = High
 * RXRDY = High
 * TXRDY = Low
 * INT = Low
 */


#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

/*
 * LINE STATUS REGISTER (LSR)
 * LSR BIT 0:
 * 0 = no data in receive holding register or FIFO.
 * 1 = data has been receive and saved in the receive holding register or FIFO.
 * ......
 * LSR BIT 5:
 * 0 = transmit holding register is full. 16550 will not accept any data for transmission.
 * 1 = transmitter hold register (or FIFO) is empty. CPU can load the next character.
 * ......
 */

#define LSR_RX_READY (1 << 0) // input is waiting to be read from RHR
#define LSR_TX_IDLE  (1 << 5) // THR can accept another character to send
#define LCR_BAUD_LATCH (1<<7) // special mode to set baud rate
#define LCR_EIGHT_BITS (3<<0)
#define uart_read_reg(reg) (*(UART_REG(reg))) // 读取对应的寄存器
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = (v)) // 写入相应内容到寄存器中,成功为1

/**
*
*/
void
uart_init()
{
    /* disable interrupts. */
	uart_write_reg(IER, 0x00);
    uint8_t lcr = uart_read_reg(LCR);
    // uart_write_reg(LCR, lcr | (1 << 7));
    // special mode to set baud rate.
    uart_write_reg(LCR,LCR_BAUD_LATCH);
    // 设置波特率
    uart_write_reg(DLL, 0x03);
    uart_write_reg(DLM, 0x00);
    // lcr = 0;
    // uart_write_reg(LCR, lcr | (3 << 0));
    
    // leave set-baud mode,
    // and set word length to 8 bits, no parity.
    uart_write_reg(LCR, LCR_EIGHT_BITS);

}

/**
* os控制uart输出一个字符
*/
int
uart_putc(char c)
{
    /*轮询读取LSR寄存器的内容，判断第五位是否为1,为1代表可以发送*/
    while((uart_read_reg(LSR) & LSR_TX_IDLE) == 0 );
    return uart_write_reg(THR, c);
}
/**
*os控制uart输出一个字符串
*/
void uart_puts(char *s)
{
    while(*s){
        uart_putc(*s++);
    }
}