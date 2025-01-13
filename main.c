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
draw_statusbar(char *status_text)
{
	int x;
	size_t status_text_len;

	/* Setup statusbar vars */
	status_text_len = strlen(status_text);

	/* Draw Statusbar */
	attron(COLOR_PAIR(1));
	mvprintw(LINES-1, 0, " NORMAL ");
	for(x = 0; x < COLS-status_text_len; x++) {
		printw(" ");
	}
	attroff(COLOR_PAIR(1));

}

void
draw_div_statusbar(char *div1, char *div2, char *div3)
{

	size_t 	div_len,
			div1_len,
			div2_len,
			div3_len,
			pad1_len,
			pad2_len,
			pad3_len
			;

	char status_text[COLS+1];

	/* Caclulate divisional markers */
	div_len = COLS / 3;
	div1_len = (div1 == NULL) ? 0 : strlen(div1);
	div2_len = (div2 == NULL) ? 0 : strlen(div2);
	div3_len = (div3 == NULL) ? 0 : strlen(div3);
	pad1_len = div_len - div1_len;
	pad2_len = div_len - div2_len;
	pad3_len = div_len - div3_len;


	/* Place status texts */
	status_text[COLS] = '\0';
	memset(status_text, ' ', COLS);
	memcpy(&status_text[0*div_len], div1, div1_len);
	memcpy(&status_text[1*div_len], div2, div2_len);
	memcpy(&status_text[2*div_len], div3, div3_len);

	/* Draw status bar */
	draw_statusbar(status_text);
}

main(void)
{
	int ch;
	int done;

	/* Setup ncurses */
	setup();

	/* Statusbar */
	draw_statusbar(" NORMAL ");

	refresh();
}
