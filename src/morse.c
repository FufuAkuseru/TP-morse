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
    // int serial_port = setup_uart();

    int           c;
    extern char  *optarg;
    char         *msg             = calloc(MSG_SIZE, sizeof(char));
    char         *output_filename = calloc(MSG_SIZE, sizeof(char));
    unsigned char frame[MSG_SIZE] = {};
    bool          s_flag          = false;
    bool          b_flag          = false;
    unsigned char repeat          = 1;
    int           timers_ms[3]    = {10, 50, 100};
    unsigned char msg_len         = 0;
    bool          default_timers  = true;
    bool          mono_timer      = false;
    bool          multi_timer     = false;

    while ((c = getopt(argc, argv, "bhm:n:st:o:T:")) != -1) {
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
                                        "(must be < 256)\nDefaulting to 255\n");
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
                if (!mono_timer) {
                    default_timers = false;
                    multi_timer    = true;
                    timers_ms[0]   = atoi(strtok(optarg, " "));
                    timers_ms[1]   = atoi(strtok(NULL, " "));
                    timers_ms[2]   = atoi(strtok(NULL, " "));
                } else {
                    fprintf(stderr, "Timers already defined by -T\n"
                                    "Using to -T values\n");
                }
                break;
            case 'T':
                if (!multi_timer) {
                    default_timers = false;
                    mono_timer     = true;
                    timers_ms[0]   = atoi(optarg);
                    timers_ms[1]   = atoi(optarg) * 3;
                    timers_ms[2]   = atoi(optarg) * 7;
                } else {
                    fprintf(stderr, "Timers already defined by -t\n"
                                    "Using to -t values\n");
                }
                break;
            case 'o':
                if (strlen(optarg) >= 255) {
                    fprintf(stderr, "Output file name too long (must be <255)");
                    exit(9);
                }
                strcpy(output_filename, optarg);
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                exit(2);
                break;
        }
    }

    if (s_flag) {
        unsigned char stop_frame[] = {0,
                                      1,
                                      (unsigned char) timers_ms[0] / 10,
                                      (unsigned char) timers_ms[1] / 10,
                                      (unsigned char) timers_ms[2] / 10,
                                      0,
                                      '\n'};
        printf("Stopping\n");
        print_frame(stop_frame, 0);
        // if (write(serial_port, stop_frame, sizeof(stop_frame)) == -1) {
        //     fprintf(stderr, "Couldn't write to serial port (%d)\n",
        //             serial_port);
        //     exit(3);
        // }
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
        printf("timer %d: %d ms\t", i, timers_ms[i]);
    }
    printf("\n");

    /* BUILDING MESSAGE TO SEND */
    frame[0] = b_flag;
    frame[1] = repeat;
    frame[2] = default_timers ? timers_ms[0] : timers_ms[0] / 10;
    frame[3] = default_timers ? timers_ms[1] : timers_ms[1] / 10;
    frame[4] = default_timers ? timers_ms[2] : timers_ms[2] / 10;
    frame[5] = msg_len;
    for (int i = 0; i < msg_len; i++) {
        frame[6 + i] = (unsigned char) msg[i];
        morse_to_beep(frame[2], frame[3], frame[4], convert_char(msg[i]));
    }
    printf("\n");
    frame[6 + msg_len] = '\n';

    if (strcmp((char *) output_filename, "") != 0) {
        FILE *file  = fopen(output_filename, "w");
        char *txt_0 = calloc(512, sizeof(char));
        sprintf(txt_0, "{ %d, %d, %d, %d, %d, %d, ", frame[0], frame[1],
                frame[2], frame[3], frame[4], frame[5]);
        for (unsigned char i = 0; i < frame[5]; ++i) {
            char *txt_1 = calloc(512, sizeof(char));
            sprintf(txt_1, "\'%c\', ", frame[6 + i]);
            strcat(txt_0, txt_1);
        }
        char *txt_2 = calloc(512, sizeof(char));
        sprintf(txt_2, "\'\\n\' }");
        strcat(txt_0, txt_2);
        fputs(txt_0, file);
        fclose(file);
    }

    print_frame(frame, msg_len);

    free(msg);
    free(output_filename);

    // if (write(serial_port, frame, msg_len + 7) == -1) {
    //     fprintf(stderr, "Couldn't write to serial port (%d)\n", serial_port);
    //     exit(5);
    // }

    return 0;
}