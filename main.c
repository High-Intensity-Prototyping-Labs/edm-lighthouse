#include <ncurses.h>

void
setup(void)
{

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

}

main(void)
{
	int ch;
	int done;

	/* Setup ncurses */
	setup();

	done = 0;
	while(!done) {
		printw("Type any character to see it in bold\n");
		ch = getch();

		switch(ch) {
			case KEY_F(1):
				printw("Pressed the F1 key");
				break;
			case 'q':
				printw("Quitting so early?");
				done = 1;
				break;
			default:
				printw("The pressed key is ");
				attron(A_BOLD);
				printw("%c", ch);
				attroff(A_BOLD);
		}

		refresh();
	}

	endwin();
}
