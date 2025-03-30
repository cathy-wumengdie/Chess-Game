#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <memory>
#include <string>
#include <vector>

#include "../enum.h"
#include "../move/move.h"
#include "../players/player.h"

// Forward declarations:
class ChessPiece;
class King;
class Player;

using namespace std;

class ChessBoard {
  const int rows = 8;
  const int cols = 8;
  vector<vector<shared_ptr<ChessPiece>>> pieces;
  Move lastMove;
  pair<shared_ptr<Player>, shared_ptr<Player>> players;

 public:
  ChessBoard();
  ChessBoard(shared_ptr<Player> player1, shared_ptr<Player> player2);

  shared_ptr<ChessPiece> getKing(Color::Color color) const;

  vector<vector<shared_ptr<ChessPiece>>> getPieces() const;

  vector<shared_ptr<ChessPiece>> getPieces(Color::Color color) const;

  // gets the captured pieces of a given color
  vector<shared_ptr<ChessPiece>> getCapturedPieces(Color::Color color) const;

  // sets a piece as captured
  void setCaptured(pair<int, int> position);

  // undoes a capture for a given position of player w/ given color
  void undoCapture(pair<int, int> position, Color::Color color);

  // sets position {row, col} to piece
  // where {row, col} = {piece.row, piece.col}
  void setPosition(pair<int, int> position, shared_ptr<ChessPiece> piece);

  // gets the piece at position {row, col}
  std::shared_ptr<ChessPiece> getPosition(pair<int, int> position) const;

  // gets the number of rows
  int getRows() const;

  // gets the number of cols
  int getCols() const;

  // moves the piece according to arguement 'move:'
  // => (1) performs side effects according to Move::SideEffects
  // which can include castling, capturing, enpassant, or none
  // => (2) updates the board according to 'move'
  // => (3) updates the piece according to 'move'
  // => (4) updates board according to side effects
  // => (5) updates affected pieces according to side effects
  void movePiece(Move move);

  // undoes a move on the board
  void undoMove(Move move);

  // checks if a given move is within the bounds of the board
  bool isMoveWithinBounds(pair<int, int> position) const;

  // gets the last move. Moreover, if no move occured, returns
  // {Move::InvalidPos, Move::InvalidPos}
  Move getLastMove() const;

  // sets lastMove to argument 'move'
  void setLastMove(Move move);

  // generates all legal moves Player (of type color) can make
  // includes checks, pins, and other edgecases
  // IMPORTANT NOTE: Differs from 'valid' moves, which are moves that
  // can be made based on the definition of the pieces, but does not
  // account for checks, pins, and other edgecases.
  vector<Move> generateLegalMoves(Color::Color color);

  // checks if a move is legal
  // returns true if the move is legal
  // returns false if the move is illegal
  bool isLegalMove(Move move, Color::Color color);

  bool anyPiecesInTheWay(pair<int,int> startPos, pair<int,int> endPos) const;
};

#endif
