#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include "utils.h"

#define MSG_SIZE 256
#define NOT_SET  -1

// void print_usage(char *prog_name) {
//     fprintf(stderr,
//             "Usage: %s [-b] [-h] [-m MSGSTRING] [-n REPEAT] [-s] [-t "
//             "TIMESTRING]\n",
//             prog_name);
//     fprintf(stderr, "\t-b: loop message\n");
//     fprintf(stderr, "\t-h: show this message\n");
//     fprintf(stderr, "\t-m MESSAGE: set the MESSAGE to send of length <
//     256\n"); fprintf(stderr, "\t-n REPEAT: repeat the message REPEAT
//     times\n"); fprintf(stderr, "\t-s: stop the program\n"); fprintf(stderr,
//     "\t-t TIMESTRING: setting up the timers with the format "
//                     "\"timer1 timer2 timer3\" as TIMESTRING\n");
// }

// int setup_uart() {
//     int serial_port = open("/dev/ttyS3", O_RDWR);

//     /* Check for errors */
//     if (serial_port < 0) {
//         printf("Error %i from open: %s\n", errno, strerror(errno));
//     }

//     /* Create new termios struct, we call it 'tty' for convention
//     No need for "= {0}" at the end as we'll immediately write the existing
//     config to this struct */
//     struct termios tty;

//     /* Read in existing settings, and handle any error
//     NOTE: This is important! POSIX states that the struct passed to
//     tcsetattr() must have been initialized with a call to tcgetattr()
//     overwise behaviour is undefined */
//     if (tcgetattr(serial_port, &tty) != 0) {
//         printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
//     }

//     /* Clear parity bit, disabling parity (most common) */
//     tty.c_cflag &= ~PARENB;
//     /* Set parity bit, enabling parity */
//     // tty.c_cflag |= PARENB;

//     /* Clear stop field, only one stop bit used in
//     communication (most common) */
//     tty.c_cflag &= ~CSTOPB;

//     /* Set stop field, two stop bits used in communication */
//     // tty.c_cflag |= CSTOPB;

//     /* Clear all the size bits, then use one of the statements below */
//     tty.c_cflag &= ~CSIZE;

//     /* 5 bits per byte */
//     // tty.c_cflag |= CS5;

//     /* 6 bits per byte */
//     // tty.c_cflag |= CS6;

//     /* 7 bits per byte */
//     // tty.c_cflag |= CS7;

//     /* 8 bits per byte (most common) */
//     tty.c_cflag |= CS8;

//     /* Disable RTS/CTS hardware flow control (most common) */
//     tty.c_cflag &= ~CRTSCTS;

//     /* Enable RTS/CTS hardware flow control */
//     // tty.c_cflag |= CRTSCTS;

//     /* Turn on READ & ignore ctrl lines (CLOCAL = 1) */
//     tty.c_cflag |= CREAD | CLOCAL;

//     /* Disable canonical mode */
//     tty.c_lflag &= ~ICANON;

//     /* Disable echo */
//     tty.c_lflag &= ~ECHO;

//     /* Disable erasure */
//     tty.c_lflag &= ~ECHOE;

//     /* Disable new-line echo */
//     tty.c_lflag &= ~ECHONL;

//     /* Disable interpretation of INTR, QUIT and SUSP */
//     tty.c_lflag &= ~ISIG;

//     /* Turn off s/w flow ctrl */
//     tty.c_iflag &= ~(IXON | IXOFF | IXANY);

//     /* Disable any special handling of received bytes */
//     tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
//     ICRNL);

//     /* Prevent special interpretation of output bytes (e.g. newline chars) */
//     tty.c_oflag &= ~OPOST;
//     /* Prevent conversion of newline to carriage return/line feed */
//     tty.c_oflag &= ~ONLCR;

//     /* Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX) */
//     // tty.c_oflag &= ~OXTABS;

//     /* Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)
//     */
//     // tty.c_oflag &= ~ONOEOT;

//     /* Wait for up to 1s (10 deciseconds), returning as soon as any data is
//      * received. */
//     tty.c_cc[VTIME] = 10;
//     tty.c_cc[VMIN]  = 0;

//     /* Set in/out baud rate to be 9600 */
//     cfsetspeed(&tty, B9600);

//     /* Save tty settings, also checking for error */
//     if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
//         printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
//     }
//     return serial_port;
// }

int main(int argc, char **argv) {
    int serial_port = setup_uart();

    int            c;
    extern char   *optarg;
    unsigned char *msg                   = calloc(MSG_SIZE, sizeof(char));
    unsigned char  msg_to_send[MSG_SIZE] = {};
    bool           s_flag                = false;
    bool           b_flag                = false;
    unsigned char  repeat                = 1;
    unsigned char  timer_ms[3]           = {10, 50, 100};
    unsigned char  msg_len               = 0;

    while ((c = getopt(argc, argv, "bhm:n:st:")) != -1) {
        switch (c) {
            case 'b':
                if (repeat != 1) {
                    fprintf(stderr, "Cannot have -n and -b\nDefaulting to -b "
                                    "as disabled\n");
                    b_flag = false;
                } else {
                    b_flag = true;
                    repeat = 0;
                }
                break;
            case 'm':
                strcpy(msg, optarg);
                msg_len = strlen(optarg);
                if (msg_len >= 249) {
                    fprintf(stderr, "Message inputed is too long (must be of "
                                    "length <249)\n");
                    exit(1);
                }

                break;
            case 'n':
                if (b_flag) {
                    fprintf(stderr, "Cannot have -b and -n\n"
                                    "Defaulting to -n as 1\n");
                    repeat = 1;
                } else {
                    repeat = atoi(optarg);
                }
                break;
            case 's':
                s_flag = true;
                break;
            case 't':
                timer_ms[0] = (char) atoi(strtok(optarg, " "));
                timer_ms[1] = (char) atoi(strtok(NULL, " "));
                timer_ms[2] = (char) atoi(strtok(NULL, " "));
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                exit(2);
                break;
        }
    }

    if (s_flag) {
        unsigned char stop_msg[] = {
            0, 1,   timer_ms[0] * 10, timer_ms[1] * 10, timer_ms[2] * 10,
            0, '\n'};
        printf("Stopping\n");
        write(serial_port, stop_msg, sizeof(stop_msg));
        exit(0);
    }

    if (strcmp(msg, "") == 0) {
        printf("No msg provided\n");
        exit(3);
    } else {
        printf("%s\n", msg);
    }

    b_flag ? printf("looping\n")
           : printf("Repeating %d time%s\n", repeat, repeat > 1 ? "s" : "");

    for (int i = 0; i < 3; ++i) {
        printf("timer %d: %d ms\t", i, timer_ms[i]);
    }
    printf("\n");

    /* BUILDING MESSAGE TO SEND */
    msg_to_send[0] = b_flag;
    msg_to_send[1] = repeat;
    msg_to_send[2] = timer_ms[0] * 10;
    msg_to_send[3] = timer_ms[1] * 10;
    msg_to_send[4] = timer_ms[2] * 10;
    msg_to_send[5] = msg_len;
    for (int i = 0; i < msg_len; i++) {
        msg_to_send[6 + i] = (char) msg[i];
    }
    msg_to_send[6 + msg_len] = '\n';

    printf("%03d", msg_to_send[0]);
    printf(" %03d", msg_to_send[1]);
    printf(" %03d", msg_to_send[2]);
    printf(" %03d", msg_to_send[3]);
    printf(" %03d", msg_to_send[4]);
    printf(" %03d ", msg_to_send[5]);
    for (int i = 0; i < msg_len; i++) {
        printf("%c", msg_to_send[6 + i]);
    }
    printf(" %03d", msg_to_send[6 + msg_len]);
    printf("%c\n", '\0');

    free(msg);

    write(serial_port, msg_to_send, msg_len + 7);

    return 0;
}