#ifndef UTILITIES_H
#define UTILITIES_H
#include <windows.h>
#include <stdio.h>

class utilities
{
    public:
        static void clean(int x, int y);
        static void write(int x, int y, char c);
    protected:

    private:
        static void gotoxy(int x, int y);
        utilities();
};

#endif // UTILITIES_H
