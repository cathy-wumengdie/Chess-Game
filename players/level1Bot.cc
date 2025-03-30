#include <iostream>

#include "level1Bot.h"

Level1Bot::Level1Bot(Color::Color color, std::string id, int level):
  ComputerPlayer{color, id, level} {}

void Level1Bot::generateMove(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  shared_ptr<ChessPiece> piece;
  do {
    piece = generateRandomPiece(chessBoard);
    std::cout << "{" << piece->getPosition().first << "," << piece->getPosition().second << "}";   //will delete later
  } while (!generateRandomMove(piece, chessBoard, oldPos, newPos));
  std::cout << " -> {" << piece->getPosition().first << "," << piece->getPosition().second << "}" << endl;      //will delete later
}

