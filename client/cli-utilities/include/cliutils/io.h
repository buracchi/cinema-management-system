#pragma once

#include <stdbool.h>
#include <setjmp.h>

extern jmp_buf leave_buff;
extern bool io_initialized;

#define get_input(question, buff, hide) get_input_len(question, sizeof(buff), buff, hide)
#define multi_choice(question, choices) multi_choice_len(question, choices, sizeof(choices))

extern char *get_input_len(char *question, int len, char *buff, bool hide);
extern bool yes_or_no(char *question, char yes, char no, bool default_answer, bool insensitive);
extern char multi_choice_len(char *question, const char choices[], int num);
extern void io_clear_screen(void);
extern void press_anykey(void);
