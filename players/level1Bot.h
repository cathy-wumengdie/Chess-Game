#include "computerPlayer.h"

class Level1Bot : public ComputerPlayer {
  public:     ///will delete later
  Level1Bot(Color::Color color, std::string id, int level);
  void generateMove(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos); //new
};
