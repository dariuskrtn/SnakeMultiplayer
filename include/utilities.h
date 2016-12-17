#ifndef UTILITIES_H
#define UTILITIES_H
#include <windows.h>
#include <stdio.h>

class utilities
{
    public:
        static void clean(int x, int y);
        static void write(int x, int y, char c);
        static void gotoxy(int x, int y);
    protected:

    private:
        utilities();
};

#endif // UTILITIES_H
