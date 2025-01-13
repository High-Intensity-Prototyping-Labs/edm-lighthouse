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
	timeout(0); /* Non-blocking reads */

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

struct menuopt {
	char *letters;
	char *name;
	char *expl;
	void (*callback)(void*);
	void *args;
};
#define MENUOPT(_letters, _name, _expl, _callback, _args) (struct menuopt) { .letters = _letters, .name = _name, .expl = _expl, .callback = _callback, .args = _args }

void
draw_menu(const char *title, struct menuopt *menuopts, size_t numopts)
{
	size_t x;

	size_t 	menuh,
			menuw,
			lstart,
			hstart
			;

	size_t 	titlepad,
			entrypad,
			entrylen
			;

	const size_t max_entrylen=64;
	char entry[max_entrylen];

	const size_t enheight=2;
	menuh = 1 + enheight*numopts; /* 2 lines per entry + title */
	menuw = strlen(title); /* Default title width */

	/* Process opts */
	for(x = 0; x < numopts; x++) {
		/* Update menu by opt max width */
		if(menuw < strlen(menuopts[x].expl)) {
			menuw = strlen(menuopts[x].expl);
		}
	}
	lstart = (COLS-menuw)/2;
	hstart = (LINES-menuh)/2;

	/* Calculate entry divs */
	titlepad = (menuw - strlen(title))/2;

	/* Draw opts */
	mvprintw(hstart, lstart+titlepad, title);
	for(x = 0; x < numopts; x++) {
		entrylen = snprintf(entry, max_entrylen, "[%s] %s - %s", menuopts[x].letters, menuopts[x].name, menuopts[x].expl);
		entrypad = (menuw-entrylen)/2;
		mvprintw(hstart+(x+1)*enheight, lstart+entrypad, entry);
	}
}

void
menu_edit(void* args)
{
	(void)args;
	mvprintw(2,0,"Seem like you tried to open the edit menu!");
}

void
menu_quit(void* args)
{
	int *done_ptr = (int*)args;

	mvprintw(2,0,"Seems like you're trying to quit");
	*done_ptr = 1;
}

main(void)
{
	int ch;
	int done;
	size_t x, xx;

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

		/* Main Menu */
		struct menuopt mmopts[] = {
			MENUOPT("eE", "Edit", "Edit edm instance", menu_edit, NULL),
			MENUOPT("q", "Quit", "Quit the edm-lighthouse", menu_quit, &done)
		};
		const size_t numopts = sizeof(mmopts) / sizeof(struct menuopt);
		draw_menu(
			"Main Menu",
			&mmopts,
			numopts
		);

		/* User interaction */
		ch = getch();
		for(x = 0; x < numopts; x++) {
			for(xx = 0; xx < strlen(mmopts[x].letters); xx++) {
				if(ch == mmopts[x].letters[xx]) {
					mmopts[x].callback(mmopts[x].args);
				}
			}
		}

		/* Refresh window */
		refresh();

		/* Time delay */
		sleep(1);
	}

}
