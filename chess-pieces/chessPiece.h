#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <memory>
#include <utility>

#include "../enum.h"

// Forward declaration:
class ChessBoard;

using namespace std;

class ChessPiece {
 protected:
  pair<int, int> position;
  Piece::Id pieceId;
  Color::Color color;
  Points::Points value;

 public:
  ChessPiece(const pair<int, int> position, const Piece::Id pieceId,
             const Color::Color color, const Points::Points value);

  virtual ~ChessPiece() = default;

  pair<int, int> getPosition() const;

  void setPosition(pair<int, int> position);

  Color::Color getColor() const;

  virtual Piece::Id getId() const = 0;

  virtual Points::Points getValue() const = 0;

  virtual char getSymbol() const = 0;
};

#endif
