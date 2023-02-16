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

    printf("%02x", msg_to_send[0]);
    printf(" %02x", msg_to_send[1]);
    printf(" %02x", msg_to_send[2]);
    printf(" %02x", msg_to_send[3]);
    printf(" %02x", msg_to_send[4]);
    printf(" %02x ", msg_to_send[5]);
    for (int i = 0; i < msg_len; i++) {
        printf("%c", msg_to_send[6 + i]);
    }
    printf(" %02x", msg_to_send[6 + msg_len]);
    printf("%c\n", '\0');

    free(msg);

    write(serial_port, msg_to_send, msg_len + 7);

    return 0;
}