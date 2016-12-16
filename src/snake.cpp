#include "snake.h"

snake::snake(char b)
{
    //ctor
    bodyChar = b;
    alive = false;
    direction = make_pair(1, 0);
}

snake::~snake()
{
    //dtor
}

void snake::spawn(pair <int, int> pos, char mapas[][200], int defaultSize)
{
    if (alive) return;
    for (int i=pos.first-defaultSize+1; i<=pos.first; i++) {
        body.push_back(make_pair(i, pos.second));
        mapas[i][pos.second] = bodyChar;
    }
}

void snake::changeDirection(pair <int, int> pos)
{
    direction = pos;
}

void snake::update(char mapas[][200])
{
    if (!keepsLiving(mapas)) kill(mapas);
    if (!alive) return;
    pair <int, int> newPos = make_pair(body[body.size()-1].first+direction.first, body[body.size()-1].second+direction.second);
    char newChar = mapas[newPos.first][newPos.second];
    mapas[newPos.first][newPos.second] = bodyChar;
    utilities::write(newPos.first, newPos.second, bodyChar);
    if (newChar!=foodChar) {
        mapas[body[0].first][body[0].second] = emptyChar;
        utilities::write(body[0].first, body[0].second, emptyChar);
        body.erase(body.begin());
    }
}

bool snake::keepsLiving(char mapas[][200])
{
    if (!alive) return false;
    pair <int, int> newPos = make_pair(body[body.size()-1].first+direction.first, body[body.size()-1].second+direction.second);
    char newChar = mapas[newPos.first][newPos.second];
    if (newChar!=foodChar && newChar!=emptyChar)
        return false;
    return true;
}

void snake::kill(char mapas[][200])
{
    if (!alive) return;
    alive = false;
    for (int i = 0; i < body.size(); i++) {
        mapas[body[i].first][body[i].second] = emptyChar;
    }
}
