#include "computerPlayer.h"

class Level2Bot : public ComputerPlayer {
 public:
  Level2Bot(Color::Color color, std::string id, int level);
  void generateMove(shared_ptr<ChessBoard>, pair<int,int>& oldPos, pair<int,int>& newPos);
};
