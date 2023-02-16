#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_SIZE 256
#define NOT_SET  -1

void print_usage(char *prog_name) {
    fprintf(stderr,
            "Usage: %s [-b] [-h] [-m MSGSTRING] [-n REPEAT] [-s] [-t "
            "TIMESTRING] \n",
            prog_name);
    fprintf(stderr, "\t-b: loop message\n");
    fprintf(stderr, "\t-h: show this message\n");
    fprintf(stderr, "\t-m MESSAGE: set the MESSAGE to send of length < 256\n");
    fprintf(stderr, "\t-n REPEAT: repeat the message REPEAT times\n");
    fprintf(stderr, "\t-s: stop the program\n");
    fprintf(stderr, "\t-t TIMESTRING: setting up the timers with the format "
                    "\"timer1 timer2 timer3\" as TIMESTRING\n");
}

int main(int argc, char **argv) {
    int          c;
    extern int   optind;
    extern char *optarg;
    char        *msg                   = calloc(MSG_SIZE, sizeof(char));
    char         msg_to_send[MSG_SIZE] = {};
    bool         s_flag                = false;
    bool         b_flag                = false;
    char         repeat                = 1;
    char         timer_ms[3]           = {10, 50, 100};
    char         msg_len               = 0;

    while ((c = getopt(argc, argv, "bhm:n:st:")) != -1) {
        switch (c) {
            case 'b':
                if (repeat != 1) {
                    fprintf(stderr, "Cannot have -n and -b\nDefaulting to -b "
                                    "as disabled\n");
                    b_flag = false;
                } else {
                    b_flag = true;
                }
                break;
            case 'm':
                strcpy(msg, optarg);
                msg_len = strlen(msg);
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
        printf("timer %d: %d\t", i, timer_ms[i]);
    }
    printf("\n");

    msg_to_send[0] = b_flag;
    msg_to_send[1] = repeat;
    msg_to_send[2] = timer_ms[0];
    msg_to_send[3] = timer_ms[1];
    msg_to_send[4] = timer_ms[2];
    msg_to_send[5] = msg_len;
    for (int i = 0; i < msg_len; i++) {
        msg_to_send[6 + i] = (char) msg[i];
    }
    msg_to_send[6 + msg_len] = '\n';

    for (int i = 0; i < MSG_SIZE; ++i) {
        printf("%d", msg_to_send[i]);
    }
    printf("%c\n", '\0');

    free(msg);

    return 0;
}