#ifndef __UTILS_H__
#define __UTILS_H__

void print_usage(char *prog_name);
int  setup_uart();
void print_frame(unsigned char *frame, unsigned char msg_len);

#endif