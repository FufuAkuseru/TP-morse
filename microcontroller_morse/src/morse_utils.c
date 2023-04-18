#include "morse_utils.h"
#include "gpio_utils.h"
#include "tim_utils.h"

#include <ctype.h>
#include <stdbool.h>

extern bool  tim2_done;
extern bool  tim3_done;
extern bool  tim4_done;
extern Pin_t led1;

/// @brief Waits a short amount of time
void short_wait() {
    start_tim(TIM2);
    while (!tim2_done) {
        ;
    }
    tim2_done = false;
    stop_tim(TIM2);
}

/// @brief Waits a medium amount of time
void medium_wait() {
    start_tim(TIM3);
    while (!tim3_done) {
        ;
    }
    stop_tim(TIM3);
}

/// @brief Waits a long amount of time
void long_wait() {
    start_tim(TIM4);
    while (!tim4_done) {
        ;
    }
    tim4_done = false;
    stop_tim(TIM4);
}

/// @brief Makes the LED turn on for a short amount of time
void short_blink() {
    set_pin(&led1);
    short_wait();
    clear_pin(&led1);
}

/// @brief Makes the LED turn on for a medium amount of time
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

/// @brief Converts a uint8_t punctuation into its morse version
/// @param c The punctuation to convert
/// @return A string of morse code
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

void string_to_morse(uint8_t *str, uint8_t str_length) {
    for (int i = 0; i < str_length; i++) {
        uint8_t current_char = str[i];
        /* for each character in the string */
        if (current_char == ' ') {
            long_wait();
        } else {
            /* converts to morse the character */
            char *morse_code;
            if (isalpha(current_char)) {
                if (current_char - 'a' > 25) {
                    morse_code = "";
                }
                morse_code = morse_letters[current_char - 'a'];
            } else if (isdigit(current_char)) {
                morse_code = morse_digits[current_char - '0'];
            } else {
                morse_code = get_morse_punct(current_char);
            }
            for (int j = 0; j < strlen(morse_code); j++) {
                /* for each dot/dash in the morse representation */
                char current_morse_char = morse_code[j];
                if (current_morse_char == '.') {
                    short_blink();
                } else if (current_morse_char == '-') {
                    medium_blink();
                }
                /* between each dot/dash waits a short amount of time */
                short_wait();
            }
        }
        /* between each letter waits a medium amount of time */
        medium_wait();
    }
}
