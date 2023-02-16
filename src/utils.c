#include "utils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>

void print_usage(char *prog_name) {
    fprintf(stderr,
            "Usage: %s [-b] [-h] [-m MSGSTRING] [-n REPEAT] [-s] [-t "
            "TIMESTRING]\n",
            prog_name);
    fprintf(stderr, "\t-b: loop message\n");
    fprintf(stderr, "\t-h: show this message\n");
    fprintf(stderr, "\t-m MESSAGE: set the MESSAGE to send of length < 256\n");
    fprintf(stderr, "\t-n REPEAT: repeat the message REPEAT times\n");
    fprintf(stderr, "\t-s: stop the program\n");
    fprintf(stderr, "\t-t TIMESTRING: setting up the timers with the format "
                    "\"timer1 timer2 timer3\" as TIMESTRING\n");
}

int setup_uart() {
    int serial_port = open("/dev/ttyS3", O_RDWR);

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

    /* Clear parity bit, disabling parity (most common) */
    tty.c_cflag &= ~PARENB;
    /* Set parity bit, enabling parity */
    // tty.c_cflag |= PARENB;

    /* Clear stop field, only one stop bit used in
    communication (most common) */
    tty.c_cflag &= ~CSTOPB;

    /* Set stop field, two stop bits used in communication */
    // tty.c_cflag |= CSTOPB;

    /* Clear all the size bits, then use one of the statements below */
    tty.c_cflag &= ~CSIZE;

    /* 5 bits per byte */
    // tty.c_cflag |= CS5;

    /* 6 bits per byte */
    // tty.c_cflag |= CS6;

    /* 7 bits per byte */
    // tty.c_cflag |= CS7;

    /* 8 bits per byte (most common) */
    tty.c_cflag |= CS8;

    /* Disable RTS/CTS hardware flow control (most common) */
    tty.c_cflag &= ~CRTSCTS;

    /* Enable RTS/CTS hardware flow control */
    // tty.c_cflag |= CRTSCTS;

    /* Turn on READ & ignore ctrl lines (CLOCAL = 1) */
    tty.c_cflag |= CREAD | CLOCAL;

    /* Disable canonical mode */
    tty.c_lflag &= ~ICANON;

    /* Disable echo */
    tty.c_lflag &= ~ECHO;

    /* Disable erasure */
    tty.c_lflag &= ~ECHOE;

    /* Disable new-line echo */
    tty.c_lflag &= ~ECHONL;

    /* Disable interpretation of INTR, QUIT and SUSP */
    tty.c_lflag &= ~ISIG;

    /* Turn off s/w flow ctrl */
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Disable any special handling of received bytes */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    /* Prevent special interpretation of output bytes (e.g. newline chars) */
    tty.c_oflag &= ~OPOST;
    /* Prevent conversion of newline to carriage return/line feed */
    tty.c_oflag &= ~ONLCR;

    /* Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX) */
    // tty.c_oflag &= ~OXTABS;

    /* Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX) */
    // tty.c_oflag &= ~ONOEOT;

    /* Wait for up to 1s (10 deciseconds), returning as soon as any data is
     * received. */
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