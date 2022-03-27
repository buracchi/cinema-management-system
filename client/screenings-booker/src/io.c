#include "io.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <setjmp.h>

#ifdef __unix__
#include <termios.h>
#endif

jmp_buf leave_buff;
bool io_initialized;

static void leave(void) {
	if(io_initialized)
		longjmp(leave_buff, 1);
	else
		exit(EXIT_SUCCESS);
}

extern char *get_input(char *question, int len, char *buff, bool hide) {
	printf("%s", question);

#ifdef __unix__
	struct termios term, oterm;

	if(hide) {
		fflush(stdout);
		if (tcgetattr(fileno(stdin), &oterm) == 0) {
			memcpy(&term, &oterm, sizeof(struct termios));
			term.c_lflag &= ~(ECHO|ECHONL);
			tcsetattr(fileno(stdin), TCSAFLUSH, &term);
		} else {
			memset(&term, 0, sizeof(struct termios));
			memset(&oterm, 0, sizeof(struct termios));
		}
	}
#else
	// Look at termio.h on MSDN to implement similar functionality on Windows
	(void)hide;
#endif

	if(fgets(buff, len, stdin) != NULL) {
		buff[strcspn(buff, "\n")] = 0;
	} else {
		printf("EOF received, leaving...\n");
		fflush(stdout);
		leave();
	}

	// Empty stdin
	if(strlen(buff) + 1 == len) {
		int ch;
		while(((ch = getchar()) != EOF) && (ch != '\n'));
		if(ch == EOF) {
			printf("EOF received, leaving...\n");
			fflush(stdout);
			leave();
		}
	}

#ifdef __unix__
	if(hide) {
		fwrite("\n", 1, 1, stdout);
		tcsetattr(fileno(stdin), TCSAFLUSH, &oterm);
	}
#endif

	return buff;
}


extern bool yes_or_no(char *question, char yes, char no, bool default_answer, bool insensitive) {
	int extra;

	// yes and no characters should be lowercase by default
	yes = (char)tolower(yes);
	no = (char)tolower(no);

	// Which of the two is the default?
	char s, n;
	if(default_answer) {
		s = (char)toupper(yes);
		n = no;
	} else {
		s = yes;
		n = (char)toupper(no);
	}

	while(true) {
		printf("%s [%c/%c]: ", question, s, n);
		extra = 0;

		char c = (char)getchar();
		char ch = 0;
		if(c != '\n') {
			while(((ch = (char)getchar()) != EOF) && (ch != '\n'))
				extra++;
		}
		if(c == EOF || ch == EOF) {
			printf("EOF received, leaving...\n");
			fflush(stdout);
			leave();
		}
		if(extra > 0)
			continue;

		// Check the answer
		if(c == '\n') {
			return default_answer;
		} else if(c == yes) {
			return true;
		} else if(c == no) {
			return false;
		} else if(c == toupper(yes)) {
			if(default_answer || insensitive) return true;
		} else if(c == toupper(no)) {
			if(!default_answer || insensitive) return false;
		}
	}
}


extern char multi_choice(char *question, const char choices[], int num) {
	//TODO
	char* possibilities = malloc(2 * num * sizeof(char));
	int i, j = 0, extra;
	for(i = 0; i < num; i++) {
		possibilities[j++] = choices[i];
		possibilities[j++] = '/';
	}
	possibilities[j - 1] = '\0'; // Remove last '/'

	while(true) {
		printf("%s [%s]: ", question, possibilities);

		extra = 0;
		char c = (char)getchar();
		if(c == '\n')
			continue;
		char ch;
		while(((ch = (char)getchar()) != EOF) && (ch != '\n'))
			extra++;
		if(c == EOF || ch == EOF) {
			printf("EOF received, leaving...\n");
			fflush(stdout);
			leave();
		}
		if(extra > 1) // Need exactly one character on stdin
			continue;

		// Check if the choice is valid
		for(i = 0; i < num; i++) {
			if(c == choices[i])
				return c;
		}
	}
}


//extern inline void clear_screen(void) {
//	// To whom it may interest: this "magic" is a sequence of escape codes from VT100 terminals:
//	// https://www.csie.ntu.edu.tw/~r92094/c++/VT100.html
//	printf("\033[2J\033[H");
//}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

void clear_screen(void) {
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

#else // !_WIN32
#include <unistd.h>
#include <term.h>

void clear_screen(void) {
	if (!cur_term) {
		int result;
		setupterm(NULL, STDOUT_FILENO, &result);
		if (result <= 0) return;
	}
	putp(tigetstr("clear"));
}
#endif

extern void press_anykey(void) {
	char c;
	puts("\nPress any key to continue...");
	while((c = (char)getchar()) != '\n');
	(void)c;
}
