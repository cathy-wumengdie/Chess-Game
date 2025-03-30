#ifndef PAWN_H
#define PAWN_H
#include <utility>

#include "chessPiece.h"

using namespace std;

class Pawn : public ChessPiece {
  bool moved;
  char symbol;

 public:
  Pawn(const pair<int, int> position, const Color::Color color);
  ~Pawn() = default;

  Piece::Id getId() const override;

  Points::Points getValue() const override;

  char getSymbol() const override;

  bool getMoved() const;

  void setMoved();
};

#endif
