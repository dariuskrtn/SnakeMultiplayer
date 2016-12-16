#include "utilities.h"

utilities::utilities()
{
    //ctor
}

void utilities::gotoxy(int x, int y) {
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

void utilities::clean(int x, int y)
{
    write(x, y, ' ');
}

void utilities::write(int x, int y, char c)
{
    gotoxy(x, y);
    printf((char*)c);
}
