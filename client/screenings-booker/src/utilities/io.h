#pragma once
#pragma warning(disable:4996)

#include <stdbool.h>
#include <setjmp.h>

extern jmp_buf leave_buff;
extern bool io_initialized;

#define initialize_io(initialized)		\
	io_initialized = true;				\
	int __ret = setjmp(leave_buff);		\
	initialized = (__ret == 0);			\

#define get_input(question, buff, hide) get_input_len(question, sizeof(buff), buff, hide)

extern char *get_input_len(char *question, int len, char *buff, bool hide);
extern bool yes_or_no(char *question, char yes, char no, bool default_answer, bool insensitive);
extern char multi_choice(char *question, const char choices[], int num);
extern void clear_screen(void);
extern void press_anykey(void);