#ifndef UART_MSG_H
#define UART_MSG_H

typedef struct uart_message_s {
    unsigned char loop;
    unsigned char nb_ite;
    unsigned char timers[3];
    unsigned char msg_size;
    unsigned char msg[256];
    unsigned char stop;
} uart_message_t;

void print_uart_message(uart_message_t *uart_msg);
int  uart_setup(int com_port);
void send_uart_message(int serial_port, uart_message_t *uart_msg);

#endif