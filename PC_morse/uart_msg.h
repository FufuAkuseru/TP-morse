#ifndef UART_MSG_H
#define UART_MSG_H

/// @brief Data structure of UART Message
typedef struct uart_message_s {
    unsigned char loop;
    unsigned char nb_ite;
    unsigned char timers[3];
    unsigned char msg_size;
    unsigned char msg[256];
    unsigned char stop;
} uart_message_t;

/// @brief Check if the timers are correctly ordered
/// @param timers The timer array from a uart_message_t
void check_timers(unsigned char *timers);

/// @brief Prints the given Message on stdout
/// @param uart_msg The message to display
void print_uart_message(uart_message_t *uart_msg);

/// @brief Sets up the UART communication using termios
/// @param com_port Which COM port the board is connected to
/// @return         The termios serial port
int uart_setup(int com_port);

/// @brief Sends a Message to the given termios serial port
/// @param serial_port The termios serial port to send to
/// @param uart_msg    The message to send
void send_uart_message(int serial_port, uart_message_t *uart_msg);


#endif