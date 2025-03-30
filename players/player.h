#ifndef PLAYER_H
#define PLAYER_H
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../chess-board/chessBoard.h"
#include "../chess-pieces/chessPiece.h"
#include "../chess-pieces/king.h"
#include "../enum.h"
#include "../move/move.h"

using namespace std;

class ChessPiece;
class Player {
 protected:
  vector<shared_ptr<ChessPiece>> chessPieces;
  vector<shared_ptr<ChessPiece>> capturedPieces;
  Color::Color color;
  string id;
  int level;         ///cathy added level
  shared_ptr<King> king;

 public:
  Player(Color::Color color, string id, int level = 0);
  virtual ~Player() = default;

  // empties the pieces the player owns
  void reset();

  // gets the color of the player
  Color::Color getColor() const;

  // gets the ID of the player
  string getID() const;

  // gets the level of the player
  int getLevel() const;

  // gets all the pieces a player owns that ARE NOT captured
  vector<shared_ptr<ChessPiece>> getPieces() const;

  // gets all the pieces a player owns that ARE captured
  vector<shared_ptr<ChessPiece>> getCapturedPieces() const;

  // creates a new piece that the player owns denoted by Piece::Id and position
  void createPiece(Piece::Id pieceId, pair<int, int> position);

  // returns a piece at a given position, otherwise returns nullptr
  shared_ptr<ChessPiece> getPiece(pair<int, int> position) const;

  // sets a piece as captured
  // internals: removes piece from 'pieces' and puts it into 'capturedPieces'
  void makeCaptured(shared_ptr<ChessPiece> piece);

  // returns a capturedPiece at a given position, otherwise returns nullptr
  shared_ptr<ChessPiece> getCapturedPiece(pair<int, int> position) const;

  // unsets a piece as captured and puts it back onto the chessPieces array
  void undoCapture(shared_ptr<ChessPiece> piece);

  // gets the player's King Piece
  shared_ptr<ChessPiece> getKing() const;

  // sets the player's King
  void setKing(shared_ptr<King> king);

  // move piece given Piece and where you want to move the piece
  // returns true if the move is successful
  // returns false if the move failed
  bool movePiece(shared_ptr<ChessPiece> piece, pair<int, int> endPosition,
                 shared_ptr<ChessBoard> chessBoard);

  // sets piece given Piece and where you want to place the piece
  // returns true if the placement is successful
  // returns false if the placement failed
  bool setPiece(shared_ptr<ChessPiece> piece, pair<int, int> endPosition,
                shared_ptr<ChessBoard> chessBoard);

  // generates and all the possible valid moves on the chessboard
  vector<Move> generateValidMoves(shared_ptr<ChessBoard> chessBoard) const;

  // checks if a move is valid
  // returns true if the move is valid
  // returns false if the move is invalid
  // IMPORTANT: A 'Valid' move is not a legal move
  // the check for validity is dependant strictly on the individual piece
  // and where it can move. For edgecases such as Pins and Checks, use:
  // isLegalMove found in the chessBoard.
  bool isValidMove(Move move, shared_ptr<ChessBoard> chessBoard) const;

  // gets the full move details of a valid move
  // REQUIRES that the argument 'move' is a valid move
  // otherwise returns Move::InvalidMove
  Move getValidMoveDetails(Move move, shared_ptr<ChessBoard> chessBoard) const;

  vector<Move> generate1PieceValidCaptures(shared_ptr<ChessPiece> piece, shared_ptr<ChessBoard> chessBoard) const;

  //virtual void generateMove(shared_ptr<ChessBoard> chessBoard) = 0;
};

#endif
