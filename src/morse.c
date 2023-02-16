#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define MSG_SIZE 256

int main(int argc, char **argv) {
    int serial_port = setup_uart();

    int           c;
    extern char  *optarg;
    char         *msg             = calloc(MSG_SIZE, sizeof(char));
    unsigned char frame[MSG_SIZE] = {};
    bool          s_flag          = false;
    bool          b_flag          = false;
    unsigned char repeat          = 1;
    unsigned char timer_ms[3]     = {10, 50, 100};
    unsigned char msg_len         = 0;
    bool          default_timers  = true;

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
                msg_len = (unsigned char) strlen(optarg);
                if (msg_len >= 249) {
                    fprintf(stderr, "Message inputed is too long"
                                    "(must be of length <249)\n");
                    exit(1);
                }

                break;
            case 'n':
                if (b_flag) {
                    fprintf(stderr, "Cannot have -b and -n\n"
                                    "Defaulting to -n as 1\n");
                    repeat = 1;
                } else {
                    int v = atoi(optarg);
                    if (v > 256) {
                        fprintf(stderr, "Number of repeat is to high"
                                        "(must be < 256)\nDefaulting to 255");
                        repeat = (unsigned char) 256;
                    } else {
                        repeat = (unsigned char) v;
                    }
                }
                break;
            case 's':
                s_flag = true;
                break;
            case 't':
                default_timers = false;
                timer_ms[0]    = (unsigned char) atoi(strtok(optarg, " "));
                timer_ms[1]    = (unsigned char) atoi(strtok(NULL, " "));
                timer_ms[2]    = (unsigned char) atoi(strtok(NULL, " "));
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                exit(2);
                break;
        }
    }

    if (s_flag) {
        unsigned char stop_frame[] = {
            0, 1,   timer_ms[0] / 10, timer_ms[1] / 10, timer_ms[2] / 10,
            0, '\n'};
        printf("Stopping\n");
        print_frame(stop_frame, 0);
        if (write(serial_port, stop_frame, sizeof(stop_frame)) == -1) {
            fprintf(stderr, "Couldn't write to serial port (%d)\n",
                    serial_port);
            exit(3);
        }
        exit(0);
    }

    if (strcmp((char *) msg, "") == 0) {
        printf("No msg provided\n");
        exit(4);
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
    frame[0] = b_flag;
    frame[1] = repeat;
    frame[2] = default_timers ? timer_ms[0] : timer_ms[0] / 10;
    frame[3] = default_timers ? timer_ms[1] : timer_ms[1] / 10;
    frame[4] = default_timers ? timer_ms[2] : timer_ms[2] / 10;
    frame[5] = msg_len;
    for (int i = 0; i < msg_len; i++) {
        frame[6 + i] = (unsigned char) msg[i];
    }
    frame[6 + msg_len] = '\n';

    print_frame(frame, msg_len);

    free(msg);

    if (write(serial_port, frame, msg_len + 7) == -1) {
        fprintf(stderr, "Couldn't write to serial port (%d)\n", serial_port);
        exit(5);
    }

    return 0;
}