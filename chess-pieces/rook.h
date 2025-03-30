#ifndef ROOK_H
#define ROOK_H
#include <utility>

#include "chessPiece.h"

using namespace std;

class Rook : public ChessPiece {
  bool moved;
  char symbol;

 public:
  Rook(const pair<int, int> position, const Color::Color color);
  ~Rook() = default;

  Piece::Id getId() const override;

  Points::Points getValue() const override;

  char getSymbol() const override;

  bool getMoved() const;

  void setMoved();
};

#endif
