#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include <iostream>
#include "../enum.h"

class Scoreboard {
     int whiteScore = 0;
     int blackScore = 0;

     std::ostream &out = std::cout;

    public:
     void updateScoreboard(Color::Color color, int value);
     void printScoreboard() const;
};

#endif