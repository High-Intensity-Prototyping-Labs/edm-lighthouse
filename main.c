#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

enum placement
{
	TOP,
	BOTTOM,
};

void
draw_statusbar_at(enum placement where, char *status_text)
{

	int x, height;
	size_t status_text_len;

	/* Setup statusbar vars */
	status_text_len = strlen(status_text);

	if(where == TOP) {
		height = 1;
	} else {
		height = LINES-1;
	}

	/* Draw Statusbar */
	attron(COLOR_PAIR(1));
	mvprintw(height, 0, status_text);
	for(x = 0; x < COLS-status_text_len; x++) {
		printw(" ");
	}
	attroff(COLOR_PAIR(1));
}

void
draw_statusbar(char *status_text)
{
	draw_statusbar_at(BOTTOM, status_text);
}

void
draw_div_statusbar_at(enum placement where, char *div1, char *div2, char *div3)
{
		size_t 	div_len,
				div1_len,
				div2_len,
				div3_len,
				pad1_len,
				pad2_len,
				pad3_len,
				halfpad,
				leftpad
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

		/* Special divisions */
		halfpad = pad2_len / 2;
		leftpad = pad3_len;

		/* Place status texts */
		status_text[COLS] = '\0';
		memset(status_text, ' ', COLS);
		memcpy(&status_text[0*div_len], div1, div1_len);
		memcpy(&status_text[1*div_len + halfpad], div2, div2_len);
		memcpy(&status_text[2*div_len + leftpad], div3, div3_len);

		/* Draw status bar */
		draw_statusbar_at(where, status_text);
}

void
draw_div_statusbar(char *div1, char *div2, char *div3)
{
	draw_div_statusbar_at(BOTTOM, div1, div2, div3);
}

void
gettimestamp(char *out, size_t maxn)
{
	time_t now;

	now = time(NULL);
	strftime(out, maxn, "%l:%M:%S %p", localtime(&now));
}

main(void)
{
	int ch;
	int done;

	const size_t timestr_maxlen = 32;
	char timestr[timestr_maxlen];

	/* Setup ncurses */
	setup();

	done = 0;
	while(!done) {

		/* Statusbar */
		gettimestamp(timestr, timestr_maxlen);
		draw_div_statusbar(NULL, NULL, timestr);

		/* Infobar */
		draw_div_statusbar_at(TOP, " edm-lighthouse", NULL, "v100 ");

		/* User interaction */
		ch = getch();
		if(ch == 'q') {
			done = 1;
			continue;
		}

		/* Refresh window */
		refresh();

		/* Time delay */
		sleep(1);
	}

}
