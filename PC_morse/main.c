
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uart_msg.h"

typedef struct settings_flag_s {
    bool b_flag;
    bool h_flag;
    bool m_flag;
    bool n_flag;
    bool s_flag;
    bool t_flag;
    bool t__flag;
    bool c_flag;
} settings_flag_t;


void print_usage(char *prog_name) {
    fprintf(stderr,
            "Usage: %s [-b] [-h] [-m MESSAGE] [-n REPEAT] [-s] [-t "
            "TIMESTRING] [-T TIMER] -c COMPORT\n",
            prog_name);
    fprintf(stderr, "\t-b: loop message\n");
    fprintf(stderr, "\t-h: show this message\n");
    fprintf(stderr, "\t-m MESSAGE: set the MESSAGE to send of length < 256\n");
    fprintf(stderr, "\t-n REPEAT: repeat the message REPEAT times\n");
    fprintf(stderr, "\t-s: stop the program\n");
    fprintf(stderr, "\t-t TIMESTRING: setting up the timers with the format "
                    "\"timer1 timer2 timer3\" as TIMESTRING\n");
    fprintf(stderr, "\t-T TIMER: setting up the timers with a single value\n");
    fprintf(stderr, "\t-c COMPORT: number of the COM port to send to\n");
}

int main(int argc, char **argv) {
    int          c;
    extern char *optarg;

    int com_port = 1;


    uart_message_t uart_msg;
    uart_msg.loop      = 0;
    uart_msg.nb_ite    = 1;
    uart_msg.timers[0] = 10;
    uart_msg.timers[1] = 50;
    uart_msg.timers[2] = 100;
    uart_msg.msg_size  = 0;
    uart_msg.stop      = '\n';
    memset(uart_msg.msg, 0, 1);

    settings_flag_t flags = {false, false, false, false,
                             false, false, false, false};

    while ((c = getopt(argc, argv, "bhm:n:st:T:c:")) != -1) {
        switch (c) {
            case 'b':
                if (flags.n_flag) {
                    fprintf(stderr, "-n and -b conflict with each other\n"
                                    "Defaulting to -b as disabled");
                    flags.b_flag  = false;
                    uart_msg.loop = 0;
                } else {
                    flags.b_flag    = true;
                    uart_msg.loop   = 1;
                    uart_msg.nb_ite = 0;
                }
                break;
            case 'm':
                flags.m_flag   = true;
                size_t msg_len = strlen(optarg);
                if (msg_len >= 249) {
                    fprintf(stderr, "Message inputed is too long"
                                    "(must be shorter than 249 characters)\n");
                    exit(1);
                }
                memcpy(uart_msg.msg, (unsigned char *) optarg, msg_len);
                uart_msg.msg_size = (unsigned char) msg_len;
                break;
            case 'n':
                if (flags.b_flag) {
                    fprintf(stderr, "-b and -n conflict with each other\n"
                                    "Defaulting -n as 1");
                } else {
                    flags.n_flag = true;
                    int v        = atoi(optarg);
                    if (v > 255) {
                        fprintf(stderr,
                                "Number of iterations is too high"
                                "(must be less than 256)\nDefaulting to 255\n");
                        uart_msg.nb_ite = 255;
                    } else {
                        uart_msg.nb_ite = (unsigned char) v;
                    }
                    uart_msg.loop = 0;
                }
                break;
            case 's':
                flags.s_flag = true;
                break;
            case 't':
                if (!flags.t__flag) {
                    flags.t_flag  = true;
                    flags.t__flag = false;
                    int v         = atoi(strtok(optarg, " "));
                    if (v > 255) {
                        fprintf(stderr,
                                "Short timer is too high"
                                "(must be less than 256)\nDefaulting to 255\n");
                        v = 255;
                    }
                    uart_msg.timers[0] = (unsigned char) v;
                    v                  = atoi(strtok(NULL, " "));
                    if (v > 255) {
                        fprintf(stderr,
                                "Medium timer is too high"
                                "(must be less than 256)\nDefaulting to 255\n");
                        v = 255;
                    }
                    uart_msg.timers[1] = (unsigned char) v;
                    v                  = atoi(strtok(NULL, " "));
                    if (v > 255) {
                        fprintf(stderr,
                                "Long timer is too high"
                                "(must be less than 256)\nDefaulting to 255\n");
                        v = 255;
                    }
                    uart_msg.timers[2] = (unsigned char) v;
                } else {
                    fprintf(stderr, "Timers already defined by -T\n");
                    flags.t_flag = false;
                }
                break;
            case 'T':
                if (!flags.t_flag) {
                    flags.t__flag = true;
                    flags.t_flag  = false;
                    int v         = atoi(optarg);
                    if (v > 255) {
                        fprintf(stderr,
                                "Short timer is too high"
                                "(must be less than 256)\nDefaulting to 255\n");
                        uart_msg.timers[0] = 255;
                    } else {
                        uart_msg.timers[0] = (unsigned char) v;
                    }
                    if (v * 3 > 255) {
                        fprintf(stderr,
                                "Short timer (%d) multiplied by 3 is too high"
                                "(must be less than 256)\nDefaulting to 255\n",
                                v);
                        uart_msg.timers[1] = 255;
                    } else {
                        uart_msg.timers[1] = (unsigned char) v * 3;
                    }
                    if (v * 7 > 255) {
                        fprintf(stderr,
                                "Short timer (%d) multiplied by 7 is too high"
                                "(must be less than 256)\nDefaulting to 255\n",
                                v);
                        uart_msg.timers[2] = 255;
                    } else {
                        uart_msg.timers[2] = (unsigned char) v * 7;
                    }
                } else {
                    fprintf(stderr, "Timers already defined by -t\n");
                    flags.t__flag = false;
                }
                break;
            case 'c':
                flags.c_flag = true;
                com_port     = atoi(optarg);
                break;
            case 'h':
            default:
                flags.h_flag = true;
                print_usage(argv[0]);
                exit(0);
                break;
        }
    }

    if (!flags.c_flag) {
        printf("No COM port provided defaulting to COM1\n\n");
    }

    int serial_port = uart_setup(com_port);

    if (flags.s_flag) {
        uart_msg.loop   = 0;
        uart_msg.nb_ite = 0;
        if (write(serial_port, &uart_msg, 2) == -1) {
            fprintf(stderr, "Couldn't write to serial port (%d)\n",
                    serial_port);
            exit(5);
        }
        uart_msg.timers[0] = 0;
        uart_msg.timers[1] = 0;
        uart_msg.timers[2] = 0;
        uart_msg.msg_size  = 0;
        memset(uart_msg.msg, 0, 1);
    }

    if (!strcmp((char *) uart_msg.msg, "") && !flags.s_flag) {
        fprintf(stderr, "No message was provided.\n"
                        "Exiting without sending anything\n");
        exit(4);
    }

    print_uart_message(&uart_msg);

    send_uart_message(serial_port, &uart_msg);
    return 0;
}