#include <ncurses.h>
#include <string.h>

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
		init_pair(1, COLOR_BLACK, COLOR_WHITE);
	}

}

void
draw_statusbar(void)
{
	int x;
	const char *status_text;
	size_t status_text_len;

	/* Setup statusbar vars */
	status_text = " NORMAL ";
	status_text_len = strlen(status_text);

	/* Draw Statusbar */
	attron(COLOR_PAIR(1));
	mvprintw(LINES-1, 0, " NORMAL ");
	for(x = 0; x < COLS-status_text_len; x++) {
		printw(" ");
	}
	attroff(COLOR_PAIR(1));

}

main(void)
{
	int ch;
	int done;

	/* Setup ncurses */
	setup();

	/* Statusbar */
	draw_statusbar();

	refresh();
}
