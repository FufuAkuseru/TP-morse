#include "morse_utils.h"
#include "gpio_utils.h"
#include "tim_utils.h"

#include <ctype.h>
#include <stdbool.h>
#include <string.h>

extern bool  tim2_done;
extern bool  tim3_done;
extern bool  tim4_done;
extern bool  stop;
extern Pin_t led1;

char *convert_char(uint8_t c) {
    if (isalpha(c)) {
        switch (tolower(c)) {
            case 'a':
                return ".-";
                break;
            case 'b':
                return "-..";
                break;
            case 'c':
                return "-.-.";
                break;
            case 'd':
                return "-..";
            case 'e':
                return ".";
                break;
            case 'f':
                return "..-.";
                break;
            case 'g':
                return "--.";
                break;
            case 'h':
                return "....";
                break;
            case 'i':
                return "..";
                break;
            case 'j':
                return ".---";
                break;
            case 'k':
                return "-.-";
                break;
            case 'l':
                return ".-..";
                break;
            case 'm':
                return "--";
                break;
            case 'n':
                return "-.";
                break;
            case 'o':
                return "---";
                break;
            case 'p':
                return ".--.";
                break;
            case 'q':
                return "--.-";
                break;
            case 'r':
                return ".-.";
                break;
            case 's':
                return "...";
                break;
            case 't':
                return "-";
                break;
            case 'u':
                return "..-";
                break;
            case 'v':
                return "...-";
                break;
            case 'w':
                return ".--";
                break;
            case 'x':
                return "-..-";
                break;
            case 'y':
                return "-.--";
                break;
            case 'z':
                return "--..";
                break;
            default:
                return "";
                break;
        }
    } else if (isdigit(c)) {
        switch (c) {
            case '0':
                return "-----";
                break;
            case '1':
                return ".----";
                break;
            case '2':
                return "..---";
                break;
            case '3':
                return "...--";
                break;
            case '4':
                return "....-";
                break;
            case '5':
                return ".....";
                break;
            case '6':
                return "-....";
                break;
            case '7':
                return "--...";
                break;
            case '8':
                return "---..";
                break;
            case '9':
                return "----.";
                break;
            default:
                return "";
                break;
        }
    } else if (isblank(c)) {
        return "/";
    } else if (ispunct(c)) {
        switch (c) {
            case '.':
                return ".-.-.-";
                break;
            case ',':
                return "--..--";
                break;
            case '?':
                return "..--..";
                break;
            case '\'':
                return ".----.";
                break;
            case '!':
                return "-.-.--";
                break;
            case '/':
                return "-..-.";
                break;
            case '(':
                return "-.--.";
                break;
            case ')':
                return "-.--.-";
                break;
            case '&':
                return ".-...";
                break;
            case ':':
                return "---...";
                break;
            case ';':
                return "-.-.-.";
                break;
            case '=':
                return "-...-";
                break;
            case '+':
                return "-...-";
                break;
            case '-':
                return "-....-";
                break;
            case '_':
                return "..--.-";
                break;
            case '\"':
                return ".-..-.";
                break;
            case '$':
                return "...-..-";
                break;
            case '@':
                return ".--.-.";
                break;
            default:
                return "";
                break;
        }
    } else {
        return "";
    }
}

void short_blink() {
    set_pin(&led1);
    start_tim(TIM2);
    while (!tim2_done) {
        ;
    }
    stop_tim(TIM2);
    tim2_done = false;
    clear_pin(&led1);
}

void medium_blink() {
    set_pin(&led1);
    start_tim(TIM3);
    while (!tim3_done) {
        ;
    }
    stop_tim(TIM3);
    tim3_done = false;
    clear_pin(&led1);
}

void short_wait() {
    clear_pin(&led1);
    start_tim(TIM2);
    while (!tim2_done) {
        ;
    }
    tim2_done = false;
    stop_tim(TIM2);
}

void long_wait() {
    clear_pin(&led1);
    start_tim(TIM4);
    while (!tim4_done) {
        ;
    }
    tim4_done = false;
    stop_tim(TIM4);
}

void morse_to_blink(char *morse) {
    while (*morse != '\0') {
        switch (*morse) {
            case '.':
                short_blink();
                break;
            case '-':
                medium_blink();
                break;
            default:
                long_wait();
                break;
        }
        short_wait();
        ++morse;
    }
}

void str_to_blink(uint8_t *str, uint8_t str_size) {
    for (uint8_t i = 0; i < str_size; ++i) {
        morse_to_blink(convert_char(str[i]));
    }
}

void uart_message_to_blink(uart_message_t *uart_msg) {
    set_tim(TIM2, uart_msg->timers[0] * 10);
    set_tim(TIM3, uart_msg->timers[1] * 10);
    set_tim(TIM4, uart_msg->timers[2] * 10);
    uint8_t str[256];
    memcpy(str, uart_msg->msg, uart_msg->msg_size);
    if (uart_msg->loop) {
        while (!stop) {
            str_to_blink(str, uart_msg->msg_size);
        }
        return;
    } else {
        for (uint8_t i = 0; i < uart_msg->nb_ite; ++i) {
            if (!stop) {
                str_to_blink(str, uart_msg->msg_size);
            }
        }
    }
}