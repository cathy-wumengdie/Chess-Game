#ifndef COMPUTER_PLAYER
#define COMPUTER_PLAYER
#include "player.h"

class ComputerPlayer : public Player {
 public:
  ComputerPlayer(Color::Color color, std::string id, int level);
  shared_ptr<ChessPiece> generateRandomPiece(shared_ptr<ChessBoard> chessBoard);
  bool generateRandomMove(shared_ptr<ChessPiece> piece, shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos);
  bool capturePiece(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos);  //if there's a chance to capture other's piece, then return true
  vector<pair<int,int>> getCheckPos(vector<pair<int,int>> preCapturePos, pair<int,int> kingPos, shared_ptr<ChessBoard> chessBoard) const;
  bool check(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos);  //if there's a chance to check the king, then return true
  vector<pair<int,int>> getPreCapturePos(Piece::Id ID, pair<int,int> attackPos, shared_ptr<ChessBoard> chessBoard);
  bool canMove2PreCapturePos(Piece::Id ID, vector<pair<int,int>> checkPos, vector<shared_ptr<ChessPiece>> pieces, shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos);
  bool moveUnderAttackPiece(Piece::Id ID, vector<shared_ptr<ChessPiece>> opponentPieces,shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos);
  bool avoidCapture(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos);  //if a piece is under attack, then move the piece to a safe position and return true
};



#endif
