#include <ncurses.h>

main(void)
{
	int ch;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	printw("Type any character to see it in bold\n");
	ch = getch();

	switch(ch) {
		case KEY_F(1):
			printw("Pressed the F1 key");
			break;
		default:
			printw("The pressed key is ");
			attron(A_BOLD);
			printw("%c", ch);
			attroff(A_BOLD);
	}

	refresh();
	getch();
	endwin();
}
