#include <iostream>
#include "../enum.h"
#include "scoreboard.h"

void Scoreboard::updateScoreboard(Color::Color color, int value) {
    if (color == Color::White) {
        whiteScore += value;
    } else {
        blackScore += value;
    }
}

void Scoreboard::printScoreboard() const {
    out << "Final score:" << std::endl;
    out << "White: " << whiteScore << std::endl;
    out << "Black: " << blackScore << std::endl;
}
