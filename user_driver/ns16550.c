#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <poll.h>
#include <stdarg.h>

typedef struct
{
	volatile uint8_t THR_REG; /* Transmitter holding reg. */
	volatile uint8_t IER_REG; /* Interrupt enable reg. */
	volatile uint8_t IIR_REG; /* Interrupt ID reg. */
	volatile uint8_t LCR_REG; /* Line control reg. */
	volatile uint8_t MCR_REG; /* Modem control reg. */
	volatile uint8_t LSR_REG; /* Line status reg. */
	volatile uint8_t MSR_REG; /* Modem status reg. */
	volatile uint8_t SCR_REG; /* Scratch reg. */
} ns16550_t;

/* Line status */
#define LSR_DR 0x01	  /* Data ready */
#define LSR_OE 0x02	  /* Overrun error */
#define LSR_PE 0x04	  /* Parity error */
#define LSR_FE 0x08	  /* Framing error */
#define LSR_BI 0x10	  /* Break interrupt */
#define LSR_THRE 0x20 /* Transmitter holding register empty */
#define LSR_TEMT 0x40 /* Transmitter empty */
#define LSR_EIRF 0x80 /* Error in RCVR FIFO */

void ns16550_tx(uintptr_t addr, unsigned char c)
{
	ns16550_t *reg = (ns16550_t *)addr;

	while ((reg->LSR_REG & LSR_THRE) == 0) ;
	reg->THR_REG = c;
}

int ns16550_rx(uintptr_t addr, unsigned char *c, uint32_t timeout)
{
	ns16550_t *reg = (ns16550_t *)addr;
	
	while ((reg->LSR_REG & LSR_DR) == 0){
		if(timeout != 0xFFFFFFFF) {
			if(timeout == 0) {
				return -1;
			}
			timeout--;
		}
	}
	if(c) {
		*c = reg->THR_REG;
	}
	return 0;
}

#define UART_LOG_BUFF_SIZE 1024
static uint8_t *NS16550_ADDR = NULL;
static char printk_string[UART_LOG_BUFF_SIZE] = {0};

static int _puts(char *str)
{
    int counter = 0;
    if (!str)
    {
        return 0;
    }
    while (*str && (counter < UART_LOG_BUFF_SIZE))
    {
        if(*str == '\n')
	        ns16550_tx((uintptr_t)NS16550_ADDR, '\r');
	    ns16550_tx((uintptr_t)NS16550_ADDR, *str++);
        counter++;
    }
    return counter;
}

int debug_log(char *fmt, ...)
{
    va_list args;
    int plen;
    va_start(args, fmt);
    plen = vsnprintf(printk_string, sizeof(printk_string)/sizeof(char) - 1, fmt, args);
    _puts(printk_string);
    va_end(args);

    return plen;
}

int read_uio_vsync(int fd_dev, int timeout_ms) {
    int ret;
    int32_t irq = 0;
    struct pollfd fds = {
        .fd = fd_dev,
        .events = POLLIN,
    };
    ret = poll(&fds, 1, timeout_ms);
    if (ret >= 1) {
        ret = read(fd_dev, &irq, sizeof(irq));
        if (ret == (ssize_t)sizeof(irq)) {
            return irq;
        }
    } 
    return -1;
}


int main(int argc, char **argv)
{
    int fd_dev = open("/dev/uio0", O_RDWR);
    if (fd_dev < 0) {
        printf("open device failed\n");
        return -1;
    }

    NS16550_ADDR = (uint8_t *)mmap(
        NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_dev, 0);
    if(NS16550_ADDR == MAP_FAILED) {
        printf("mmap failed\n");
        return -1;
    }

    debug_log("hello world\n");

    munmap(NS16550_ADDR, 0x1000);
    close(fd_dev);

    return 0;
}