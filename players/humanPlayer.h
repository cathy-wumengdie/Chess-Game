#ifndef HUMAN_PLAYER
#define HUMAN_PLAYER

#include "player.h"

class HumanPlayer : public Player {
  HumanPlayer(Color::Color color, std::string id, int level = 0);
};

#endif
