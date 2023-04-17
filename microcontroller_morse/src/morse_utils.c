#include "morse_utils.h"
#include "gpio_utils.h"
#include "tim_utils.h"

#include <ctype.h>
#include <stdbool.h>

extern bool  tim2_done;
extern bool  tim3_done;
extern bool  tim4_done;
extern Pin_t led1;

void short_wait() {
    start_tim(TIM2);
    while (!tim2_done) {
        ;
    }
    tim2_done = false;
    stop_tim(TIM2);
}

void medium_wait() {
    start_tim(TIM3);
    while (!tim3_done) {
        ;
    }
    stop_tim(TIM3);
}

void long_wait() {
    start_tim(TIM4);
    while (!tim4_done) {
        ;
    }
    tim4_done = false;
    stop_tim(TIM4);
}

void short_blink() {
    set_pin(&led1);
    short_wait();
    clear_pin(&led1);
}

void medium_blink() {
    set_pin(&led1);
    medium_wait();
    clear_pin(&led1);
}

char *morse_letters[] = {"---",  "-...", "-.-.", "-..",  ".",    "..-.", "--.",
                         "....", "..",   ".---", "-.-",  ".-..", "--",   "-.",
                         "---",  ".--.", "--.-", ".-.",  "...",  "-",    "..-",
                         "...-", ".--",  "-..-", "-.--", "--.."};
char *morse_digits[]  = {"-----", ".----", "..---", "...--", "....-",
                         ".....", "-....", "--...", "---..", "----."};

char *get_morse_punct(uint8_t c) {
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
}

void string_to_morse(uint8_t *str, int str_length) {
    for (int i = 0; i < str_length; i++) {
        uint8_t current_char = str[i];
        if (current_char == ' ') {
            long_wait();
        } else {
            char *morse_code;
            if (isalpha(current_char)) {
                morse_code = morse_letters[current_char - 'a'];
            } else if (isdigit(current_char)) {
                morse_code = morse_digits[current_char - '0'];
            } else {
                morse_code = get_morse_punct(current_char);
            }
            for (int j = 0; j < strlen(morse_code); j++) {
                char current_morse_char = morse_code[j];
                switch (current_morse_char) {
                    case '.':
                        short_blink();
                        break;
                    case '-':
                    default:
                        medium_blink();
                }
                short_wait();
            }
        }
        medium_wait();
    }
}
