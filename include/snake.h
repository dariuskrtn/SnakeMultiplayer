#ifndef SNAKE_H
#define SNAKE_H
#include <vector>
#include <utilities.h>

using namespace std;

class snake
{
    public:
        snake(char b);
        virtual ~snake();
        void spawn(pair <int, int> pos, char mapas[][200], int defaultSize);
        void changeDirection(pair <int, int> pos);
        void update(char mapas[][200]);
        bool keepsLiving(char mapas[][200]);
        void kill(char mapas[][200]);
    protected:

    private:
        char bodyChar, foodChar, emptyChar;
        vector <pair<int, int> > body;
        pair<int, int> direction;
        bool alive;
};

#endif // SNAKE_H
