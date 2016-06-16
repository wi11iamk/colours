#include <ncurses.h>

int main()
{	
	initscr();			
	printw("Small GUI says Hello.");	
	refresh();			
	getch();			
	endwin();			

	return 0;
}
