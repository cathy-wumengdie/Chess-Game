#include "level2Bot.h"

Level2Bot::Level2Bot(Color::Color color, std::string id, int level)
    : ComputerPlayer{color, id, level} {}

void Level2Bot::generateMove(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  if (capturePiece(chessBoard,oldPos,newPos)) return; 
  //if there's no chance to capture opponent's piece, then check if there's a chance to check
  if (check(chessBoard,oldPos,newPos)) return;
  //if there's no chance to check, then give a random move
  shared_ptr<ChessPiece> piece;
  do {
    piece = generateRandomPiece(chessBoard);
    std::cout << "{" << oldPos.first << "," << oldPos.second << "}";   //will delete later
  } while (!generateRandomMove(piece, chessBoard, oldPos, newPos));
  std::cout << " -> {" << newPos.first << "," << newPos.second << "}" << endl;   //will delete later
}
