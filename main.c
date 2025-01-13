#include <ncurses.h>

void
setup(void)
{
	/* Core Setup */

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();

	/* Colours */
	if(has_colors()) {
		start_color();
		init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	}

}

main(void)
{
	int ch;
	int done;

	/* Setup ncurses */
	setup();

	/* Draw Statusbar */
	attron(COLOR_PAIR(1));
	mvprintw(LINES-1, 0, " NORMAL ");
	attroff(COLOR_PAIR(1));

	refresh();
}
