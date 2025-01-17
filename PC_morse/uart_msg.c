#include "uart_msg.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

void check_timers(unsigned char *timers) {
    if (timers[2] <= timers[1]) {
        fprintf(stderr,
                "Long timer (%d) <= medium timer (%d). "
                "Using default timer values\n",
                timers[2], timers[1]);
        timers[0] = 10;
        timers[1] = 50;
        timers[2] = 100;
        return;
    }
    if (timers[2] <= timers[0]) {
        fprintf(stderr,
                "Long timer (%d) <= short timer (%d). "
                "Using default timer values\n",
                timers[2], timers[0]);
        timers[0] = 10;
        timers[1] = 50;
        timers[2] = 100;
        return;
    }
    if (timers[1] <= timers[0]) {
        fprintf(stderr,
                "Medium timer (%d) <= short timer (%d). "
                "Using default timer  values\n",
                timers[1], timers[0]);
        timers[0] = 10;
        timers[1] = 50;
        timers[2] = 100;
        return;
    }
}

void print_uart_message(uart_message_t *uart_msg) {
    if (uart_msg->loop) {
        printf("\tLooping\n");
    } else {
        printf("\t%d iteration%s\n", uart_msg->nb_ite,
               uart_msg->nb_ite == 1 ? "" : "s");
    }
    printf("\tShort timer: %d\n", uart_msg->timers[0]);
    printf("\tMedium timer: %d\n", uart_msg->timers[1]);
    printf("\tLong timer: %d\n", uart_msg->timers[2]);
    printf("\tMessage size: %d\n", uart_msg->msg_size);
    printf("\tMessage:\n");
    for (unsigned char i = 0; i < uart_msg->msg_size; ++i) {
        printf("%c", uart_msg->msg[i]);
    }
    printf("\n");
}

int uart_setup(int com_port) {
    char com_file[11];
    sprintf(com_file, "/dev/ttyS%d", com_port - 1);
    int serial_port = open(com_file, O_RDWR);

    /* Check for errors */
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
    }

    /* Create new termios struct, we call it 'tty' for convention
    No need for "= {0}" at the end as we'll immediately write the existing
    config to this struct */
    struct termios tty;

    /* Read in existing settings, and handle any error
    NOTE: This is important! POSIX states that the struct passed to
    tcsetattr() must have been initialized with a call to tcgetattr()
    overwise behaviour is undefined */
    if (tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    /* Disable parity */
    tty.c_cflag &= ~PARENB;

    /* One stop bit used in communication */
    tty.c_cflag &= ~CSTOPB;

    /* Set to 8 bits per byte */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    /* Disable RTS/CTS hardware flow control */
    tty.c_cflag &= ~CRTSCTS;

    /* Turn on READ & ignore ctrl lines (CLOCAL = 1) */
    tty.c_cflag |= CREAD | CLOCAL;

    /* Disables canonical mode, echo, erasure, new-line echo, interpretation of INTR/QUIT/SUS */
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);

    /* Turn off s/w flow ctrl */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Disable any special handling of received bytes */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    /* Prevent special interpretation of output bytes & CRCF */
    tty.c_oflag &= ~(OPOST | ONLCR);

    /* Wait for up to 1s (10 deciseconds), returning as soon as any data is
       received. */
    tty.c_cc[VTIME] = 10;
    tty.c_cc[VMIN]  = 0;

    /* Set in/out baud rate to be 9600 */
    cfsetspeed(&tty, B9600);

    /* Save tty settings, also checking for error */
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    return serial_port;
}

void send_uart_message(int serial_port, uart_message_t *uart_msg) {
    printf("\tFrame sent:\n");
    printf("%d ", uart_msg->loop);
    printf("%d ", uart_msg->nb_ite);
    printf("%d ", uart_msg->timers[0]);
    printf("%d ", uart_msg->timers[1]);
    printf("%d ", uart_msg->timers[2]);
    printf("%d\n", uart_msg->msg_size);
    for (unsigned char i = 0; i < uart_msg->msg_size; ++i) {
        printf("%d ", uart_msg->msg[i]);
    }
    printf("%d\n", uart_msg->stop);
    if (write(serial_port, uart_msg, uart_msg->msg_size + 7) == -1) {
        fprintf(stderr, "Couldn't write to serial port (%d)\n", serial_port);
        exit(5);
    }
}