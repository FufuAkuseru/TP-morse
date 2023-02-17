#ifndef __UTILS_H__
#define __UTILS_H__

void  print_usage(char *prog_name);
int   setup_uart();
void  print_frame(unsigned char *frame, unsigned char msg_len);
char *convert_char(char c);

void morse_to_beep(unsigned char short_delay, unsigned char medium_delay,
                   unsigned char long_delay, char *code);

#endif