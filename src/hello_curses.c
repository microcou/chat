#include <locale.h>
#include <curses.h>

int main(void)
{
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();

    printw("Bonjour, éèàçù !");
    refresh();

    getch();
    endwin();

    return 0;
}