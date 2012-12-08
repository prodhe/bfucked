#include <ncurses.h>
#include <string.h>

int main()
{
	// basic vars
	int x, y;
	
	// activate ncurses
	initscr();
	
	// disable line buffering for input (and use raw as in do nothing for CTRL+C etc)
	raw();
	
	// use function keys (F1, F2, arrow keys) etc
	keypad(stdscr, TRUE);
	
	// disable echo from input typing
	noecho();
	
	// get current console size
	getmaxyx(stdscr, y, x);
	
	// bold title output
	attron(A_BOLD);
	mvprintw(1, 1, "%s", "BFUCKED");
	attroff(A_BOLD);
	mvprintw(1, 9, "%s", "using ncurses");
	
	// bottom menu
	mvaddstr(y-1, 1, "[^L] Load file     [^R] Run     [^D] Debug     [^C] Exit");
	
	// get cursor out of the way
	move(y/2,x/2);
	
	// loop input action
	char ch;
	while((ch = getch()) != 3)	// CTRL+C
	{
		switch(ch)
		{
			case 12:
			printw("LOAD");
			break;
		}
	}
	
	// update windows
	refresh();
	
	
	// end ncurses mode
	endwin();
	
	return 0;
}